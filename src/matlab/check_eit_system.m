function [HDR,Vfull]=check_eit_system(fname)
% check_eit_system
%
%   Plots some sanity checks for the parallel EIT system
%
%   Requires LoadData repo


if exist('fname','var') ==0 || isempty(fname)
    HDR=ScouseTom_getHDR;
else
    HDR=ScouseTom_getHDR(fname);
end

%% Load a max of 3 seconds of data

SecondsToLoad=floor(HDR.SPR/HDR.SampleRate);
MaxLength=3;
SecondsToLoad=min([MaxLength SecondsToLoad]);

V=sread(HDR,SecondsToLoad,0);
Fs=HDR.SampleRate;
t=(0:(length(V)-1))/Fs;
%%
%normally we want to plot
if exist('PlotFlag','var') ==0 || isempty(PlotFlag)
    PlotFlag =1;
end
fname=HDR.FILE.Name;

%% Check number of channels
% get the chan labels from actichamp software
Chn_labels= str2double(HDR.Label);

if any(isnan(Chn_labels))
    Chn_labels=1:size(HDR.Label);
end


% number of channels recorded
Chn_total=max(size(Chn_labels));

% maximum channel number
Chn_max = max(Chn_labels);

gap_in_chn = find((diff(Chn_labels) > 1));

if isempty(gap_in_chn)
    ref_chn = Chn_max +1;
else
    ref_chn = Chn_labels(gap_in_chn) + 1;
end

disp('------------------------------------');
fprintf('Recorded %d channels %d-%d, with reference %d\n',Chn_total,min(Chn_labels),Chn_max,ref_chn);

%% Check saturation

Vave=mean(V);
V_Max=4e5;
V_Rec=2e3;
% Classify channels
%Each impedance is eithenr good bad or ok
bad_idx = find(abs(Vave) > V_Max);
ok_idx=find(abs(Vave) > V_Rec & abs(Vave) < V_Max);
good_idx =find(abs(Vave) < V_Rec);

badchn=nan(size(Vave));
goodchn=badchn;
okchn=badchn;

okchn(ok_idx)=Vave(ok_idx);
goodchn(good_idx)=Vave(good_idx);
badchn(bad_idx)=Vave(bad_idx);

numbad=length(bad_idx);
numok=length(ok_idx);

%output to user
disp('------------------------------------');
fprintf('Found ');
if numbad
    
    fprintf(2,'%d bad electrodes ',numbad);
    fprintf('and ');
end

fprintf('%d warning electrodes\n',numok);
if numbad
    fprintf(2,'BAD ELECS : ');
    fprintf(2,'%d,',Chn_labels(bad_idx));
    fprintf(2,'\n');
end

if numok
    fprintf('Warning elecs : ');
    fprintf('%d,',ok_idx);
    fprintf('\n');
end

if PlotFlag
    
    figure
    title(sprintf('Mean Voltage in %s. Resistor Phantom should be <1000uV',fname),'interpreter','none');
    
    hold all
    %plot each set
    bar(Chn_labels, goodchn,'Facecolor',[0 0.5 0]);
    bar(Chn_labels,okchn,'Facecolor','y');
    bar(Chn_labels,badchn,'Facecolor',[1 0 0]);
    
    %make indication lines for recomended and max impedances
    line([0 Chn_max+1],[V_Max V_Max],'color','r','linewidth',1)
    line([0 Chn_max+1],[-V_Max -V_Max],'color','r','linewidth',1)
    %         line([0 Chn_max+1],[V_Rec V_Rec],'color','y','linewidth',5)
    %         text(1,V_Rec,'FUZZY LOGIC OK','BackgroundColor',[1 1 1],'color',[0 0 0])
    text(1,V_Max,'Sat','color','r','BackgroundColor',[1 1 1])
    text(1,-V_Max,'Sat','color','r','BackgroundColor',[1 1 1])
    hold off
    set(gca,'Xtick',[1:Chn_max])
    xlim([0,Chn_max+1])
    
    xlabel('Electrode');
    ylabel('mean(V)');
    
    
    %make plot wider
    set(gcf,'units','normalized','outerposition',[0 0 1 1])
    drawnow
end

%% Check injection channels
[Injs, Freqs] = Find_Injection_Freqs_And_Elecs(V,Fs,Chn_labels);
N_freqs=length(Freqs);

%% Plot expected voltages

BW=250;
FilterLength=0.2*Fs;

Vfull=nan(size(V,1),N_freqs*Chn_total);
Pfull=Vfull;

BV_inj=nan(Chn_total,N_freqs);
STD_inj=BV_inj;

for iFreq = 1:N_freqs
    fprintf('Processing freq %d\n',iFreq);
%     [cur_trim_demod,cur_Filt,cur_Fc]=ScouseTom_data_GetFilterTrim(V(:,Injs(iFreq,1)),Fs,BW,FilterLength,Freqs(iFreq) );
    
    cur_Filt =designfilt('bandpassiir', 'FilterOrder', 20, ...
                'HalfpowerFrequency1',Freqs(iFreq)-BW/2 ,...
                'HalfpowerFrequency2',Freqs(iFreq)+BW/2,...
                'DesignMethod','butter',...
                'SampleRate', Fs);
    
    
    %make it consistent with multifreq bits, which are all cells
    Filt{iFreq}=cur_Filt;
    TrimDemod{iFreq}=FilterLength;
    Fc{iFreq}=Freqs(iFreq);
    
    [Vdemod,Pdemod]=ScouseTom_data_DemodHilbert(V,cur_Filt);
    vidx=(iFreq-1)*Chn_total + 1:(iFreq)*Chn_total;
    
    Vfull(:,vidx)=Vdemod;
    Pfull(:,vidx)=Pdemod;
    
    BV_inj(:,iFreq)=mean(Vdemod(TrimDemod{iFreq}:end-TrimDemod{iFreq},:),1);
    STD_inj(:,iFreq)=std(Vdemod(TrimDemod{iFreq}:end-TrimDemod{iFreq},:),1);
    
end

max_trimsamples = max(cellfun(@(x) max(x),TrimDemod));

Vfull([1:max_trimsamples end-max_trimsamples:end],:)=nan;
Pfull([1:max_trimsamples end-max_trimsamples:end],:)=nan;

disp('Processing Done');


%%

if PlotFlag
    figure
    plot(Chn_labels,BV_inj,'-o')
    xlabel('Electrode');
    ylabel('mean(uV)');
    legend(num2str(round(Freqs,-1)))
    title('Boundary Voltage at each frequency')
    
    set(gcf,'units','normalized','outerposition',[0 0 1 1])
    
    figure
    plot(Chn_labels,STD_inj,'-o')
    xlabel('Electrode');
    ylabel('Noise(uV)');
    legend(num2str(round(Freqs,-1)))
    title('Noise in all channels at each frequency')
    
    set(gcf,'units','normalized','outerposition',[0 0 1 1])
    
    
    figure
    c=lines(N_freqs);
    
    rows=3;
    cols=ceil(N_freqs/rows);
    
    for iFreq = 1:N_freqs
        subplot(rows,cols,iFreq)
        plot(Chn_labels,BV_inj(:,iFreq),'o-','Color',c(iFreq,:))
        title(sprintf('%d (%d) Hz',round(Freqs(iFreq)),round(Freqs(iFreq),-2)))
        xlabel('Electrode');
        ylabel('mean(uV)');
    end
    
    set(gcf,'units','normalized','outerposition',[0 0 1 1])
    
    drawnow
    
    
end


end