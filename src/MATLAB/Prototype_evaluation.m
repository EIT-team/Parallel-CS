

%% Set up fixed parameters
Freqs = [1000 2000 3000];
Prt = [1 2; 11 19; 17 31];
N_Freqs = size(Freqs,2);

%Filter specs
Filt_Band = 50;
Filt_Order = 5;

%Get files to use
Files = dir('*.bdf');
N_Files = size(Files,1);

%intialise cells for final data storage
for i = 1:N_Freqs
    Y{i} = [];
end


%% Too much data to load into memory at the same time, so load it file by file and take out only every 5000th sample
for k = 1:N_Files
    
    
    clear EEG Data Y1;
    
    disp(['Processing file: ' Files(k).name])
    EEG = pop_biosig(Files(k).name);
    Fs = EEG.srate;
    Data =  double(EEG.data');
    
    clear EEG;
    for j = 1:N_Freqs
        
        Fc = Freqs(j);
        disp(['Frequency: ' int2str(Fc)])
        [b,a] = butter(Filt_Order,(Fc+[-Filt_Band,Filt_Band])./(Fs/2));
        
        Y1 = zeros(size(Data));
        
        for i = 1:size(Data,2)
            %X1(:,i) = filtfilt(b,a,Data(:,i));
            Y1(:,i) = abs(hilbert(filtfilt(b,a,Data(:,i))));
        end
        
        Y{j} = [];
        
        Y{j} =  [Y{j}; Y1(Fs:5000:end-Fs,:)];
        
    end
    
end

%% Plot data

for i = 1:N_Freqs
    
    subplot(N_Freqs,1,i)
    plot(Xind,1e-3*Y{i}(:,setdiff(1:32,Prt(i,:))))
    ylabel(['Voltage (mV) Freq = ' int2str(Freqs(i))])
end
    xlabel('Time (minutes)')

