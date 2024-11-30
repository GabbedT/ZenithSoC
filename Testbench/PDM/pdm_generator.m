% Parameters

% PDM sampling rate (3.072 MHz)
fs = 3072000;

Fout = 48000;           % Output sampling frequency
R = 64;          % Decimation factor
N = 2;                  % Number of CIC stages (order of the filter)
M = 5;                  % Delay
G = (R * 1)^N;


%% Sine
f_sine = 40000;
duration = 0.1;
amplitude = 0.5;

% Generate sine wave
t = (0:1/fs:duration-1/fs)';
sine_wave = amplitude * sin(2 * pi * f_sine * t);

%% Variable Sine
% Parametri
f1 = 60000;    % Frequenza finale in Hz (500 kHz)
t_totale = 10;        % Tempo totale in secondi
f_samping = 3.072e6; % Frequenza di campionamento in Hz (3.072 MHz)

% Calcola il vettore del tempo con la frequenza di campionamento
t = (0:1/f_samping:t_totale-1/f_samping); % Vettore tempo con passo di 1/f_samping

% Frequenza che cresce linearmente nel tempo (da 0 a 500 kHz)
f0 = 40000;              % Frequenza iniziale (0 Hz)

% Crea il chirp
chirp_signal = dsp.Chirp('InitialFrequency', f0, ...
                         'TargetFrequency', f1, ...
                         'TargetTime', 1, ...  % Durata del segnale (1 secondo)
                         'SweepDirection', 'unidirectional', ...  % Chirp crescente
                         'SamplesPerFrame', length(t), ...
                         'SampleRate', fs);

% Genera il segnale
x = chirp_signal();    % Il segnale chirp

% Calcola il segnale seno
var_sine_wave = chirp_signal();  % Segnale seno con fase variabile nel tempo


%% Audio
[audio_signal, Fs] = audioread('Audio.wav');  % Replace with your file
audio_signal = audio_signal(:, 1);  % Use mono channel

% Normalize audio to [0, 1]
normalized_audio = (audio_signal - min(audio_signal)) / (max(audio_signal) - min(audio_signal));

% Resample to PDM rate
oversampled_audio = resample(normalized_audio, fs, Fs);




% Normalize sine wave to range [0, 1]
normalized_wave = var_sine_wave;

% Delta-Sigma Modulation to generate PDM
pdm = zeros(size(normalized_wave));
accumulator = 0;

for i = 1:length(normalized_wave)
    accumulator = accumulator + normalized_wave(i) + 0.5;
    if accumulator > 0
        pdm(i) = 1;
    else
        pdm(i) = 0;
    end

    accumulator = accumulator - pdm(i);
end


% Plot original and PDM
t_audio = (0:length(normalized_wave)-1) / Fs;
t_pdm = (0:length(pdm)-1) / fs;
% Creazione della figura
figure;

% Primo sotto-grafico: segnale originale nel dominio del tempo
subplot(2, 1, 1);
plot(t_audio, normalized_wave, 'b');
title('Original Audio Signal');
xlabel('Time (s)');
ylabel('Amplitude');

% Limita l'asse X per visualizzare una porzione specifica
xlim([2 3]);  % Imposta l'asse x tra 2 e 3 secondi, ad esempio

% figure;
% subplot(2, 1, 1);
% plot(t, normalized_wave, 'b', 'LineWidth', 1.5);
% title('Sine Wave');
% xlabel('Time (s)');
% ylabel('Amplitude');
% xlim([t(1), t(end)]);
% grid on;

% % Plot PDM signal (digital impulses)
% figure;
% subplot(2, 1, 1);
% plot(t, pdm, 'r', 'LineWidth', 1.5);
% title('PDM Signal');
% xlabel('Time (s)');
% ylabel('PDM Output');
% ylim([-0.5, 1.5]);  % Ensure binary levels are visible
% xlim([t(1), t(end)]);
% grid on;

% Write the PDM stream to a file
fileID = fopen('pdm.txt', 'w');
fprintf(fileID, '%d', pdm);
fclose(fileID);



%% CIC FILTER %%

% Create CIC Decimator
cicFilter = dsp.CICDecimator(R, 1, N);  % Use double precision for simplicity

% Apply CIC filter to PDM signal
filtered_signal = cicFilter(pdm);


% Generate time vector for output signal
t_output = (0:length(filtered_signal)-1)' / Fout;
% Filtered signal (CIC output)
figure;
subplot(3, 1, 2);
plot(t_output, filtered_signal, 'r', 'LineWidth', 1.5);
title('Filtered Signal (CIC Output)');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;



% Create CIC Decimator
cicFilter = dsp.CICDecimator(R, 2, N);  % Use double precision for simplicity

% Apply CIC filter to PDM signal
filtered_signal = cicFilter(pdm);


% Generate time vector for output signal
t_output = (0:length(filtered_signal)-1)' / Fout;
% Filtered signal (CIC output)
figure;
subplot(3, 1, 2);
plot(t_output, filtered_signal, 'r', 'LineWidth', 1.5);
title('Filtered Signal (CIC Output)');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

% Create CIC Decimator
cicFilter = dsp.CICDecimator(R, 3, N);  % Use double precision for simplicity

% Apply CIC filter to PDM signal
filtered_signal = cicFilter(pdm);


% Generate time vector for output signal
t_output = (0:length(filtered_signal)-1)' / Fout;
% Filtered signal (CIC output)
figure;
subplot(3, 1, 2);
plot(t_output, filtered_signal, 'r', 'LineWidth', 1.5);
title('Filtered Signal (CIC Output)');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

% Create CIC Decimator
cicFilter = dsp.CICDecimator(R, 4, N);  % Use double precision for simplicity

% Apply CIC filter to PDM signal
filtered_signal = cicFilter(pdm);


% Generate time vector for output signal
t_output = (0:length(filtered_signal)-1)' / Fout;
% Filtered signal (CIC output)
figure;
subplot(3, 1, 2);
plot(t_output, filtered_signal, 'r', 'LineWidth', 1.5);
title('Filtered Signal (CIC Output)');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;


% Create CIC Decimator
cicFilter = dsp.CICDecimator(R, 5, N);  % Use double precision for simplicity

% Apply CIC filter to PDM signal
filtered_signal = cicFilter(pdm);


% Generate time vector for output signal
t_output = (0:length(filtered_signal)-1)' / Fout;
% Filtered signal (CIC output)
figure;
subplot(3, 1, 2);
plot(t_output, filtered_signal, 'r', 'LineWidth', 1.5);
title('Filtered Signal (CIC Output)');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;


% Convert to 16-bit PCM format
pcm_output = int16((filtered_signal / G) * (2^15 - 1));  % Scale to 16-bit signed integer range

% Write PCM samples to a file
output_file = 'pcm.txt';
fileID = fopen(output_file, 'w');

for i = 1:length(pcm_output)
    fprintf(fileID, '%d\n', pcm_output(i));
end

fclose(fileID);
