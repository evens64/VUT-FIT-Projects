% ISS Projekt by Jakub Frýz (xfryzj01)

clc;            % clear command window
clear;          % clear workspace
close all;      % close all figures

%--------------------------------------------------------------------------

fprintf('1. úkol\n');

[signal, fs] = audioread('xfryzj01.wav');                                  % naètení signálu

fprintf('\tVzorkovací frekvence Fs = %d kHz\n', fs/1000);                  % vzorkovací frekvence

samples = numel(signal);
fprintf('\tDélka signálu: %d s (%d vzorkù)\n', samples/fs, samples);       % délka ve vzorcích

%--------------------------------------------------------------------------

fprintf('2. úkol\n');

dft = fft(signal);                                                         % DFT nad naèteným signálem
dft_abs = abs(dft);

figure('Name', 'Discrete Fourier Transform plot of signal');               % nový graf
fprintf('\t[Graf se zobrazil...]\n');

plot(dft_abs(1:samples/2));                                                % zobrazení grafu
title('Discrete Fourier Transform plot of signal');
xlabel('Frequency [Hz]');
ylabel('Magnitude');

print('plot_dft.bmp', '-dbmp');                                            % uložení grafu

%--------------------------------------------------------------------------

fprintf('3. úkol\n');

[dft_max, dft_max_index] = max(dft_abs);                                   % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', dft_max, dft_max_index);

%--------------------------------------------------------------------------

fprintf('4. úkol\n');

b = [0.2324, -0.4112, 0.2324];                                             % b koeficienty
a = [1, 0.2289, 0.4662];                                                   % a koeficienty

[z, p, k] = tf2zpk(b,a);                                                   % nuly, póly, gain

figure('Name', 'Zero/Pole plot of IIR filter');                            % nový graf
fprintf('\t[Graf se zobrazil...]\n');

zplane(z, p);                                                              % zobrazení grafu
title('Zero/Pole plot of IIR filter');
xlabel('Real Part');
ylabel('Imaginary Part');
legend('Zero', 'Pole');

print('plot_zp.bmp', '-dbmp');                                             % uložení grafu

if isstable(b, a) == true                                                  % stabilita
    fprintf('\tFiltr je stabilní\n');
else
    fprintf('\tFiltr není stabilní\n');
end

%--------------------------------------------------------------------------

fprintf('5. úkol\n');

[h, w] = freqz(b, a, fs/2);                                                % kmitoètová charakteristika filtru

figure('Name', 'Frequency response of digital filter');                    % nový graf
fprintf('\t[Graf se zobrazil...]\n');

plot((w*fs)/(2*pi), abs(h));                                               % zobrazení grafu
ylim([0 1])
title('Frequency response of digital filter');
xlabel('Frequency (Hz)');
ylabel('Magnitude');

print('plot_iir.bmp', '-dbmp');                                            % uložení grafu

%--------------------------------------------------------------------------

fprintf('6. úkol\n');

signalf = filter(b, a, signal);                                            % filtrovaný signál
dftf = fft(signalf);                                                       % DFT nad filtrovaným signálem
dftf_abs = abs(dftf);

figure('Name', 'Discrete Fourier Transform plot of filtered signal');      % nový graf
fprintf('\t[Graf se zobrazil...]\n');

plot(dftf_abs(1:samples/2));                                               % zobrazení grafu
title('Discrete Fourier Transform plot of filtered signal');
xlabel('Frequency [Hz]');
ylabel('Magnitude');

print('plot_dftf.bmp', '-dbmp');                                           % uložení grafu

%--------------------------------------------------------------------------

fprintf('7. úkol\n');

[dftf_max, dftf_max_index] = max(abs(dftf));                               % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', dftf_max, dftf_max_index);

%--------------------------------------------------------------------------

fprintf('8. úkol\n');

find_filter = repmat([1, 1, 1, 1, -1, -1, -1, -1], 1, 40);                 % 40 sekvencí [h h h h -h -h -h -h]

find_signal = filter(find_filter, 1, signal);

figure;                                                                    % nový graf
fprintf('\t[Graf se zobrazil...]\n');

plot(find_signal);                                                         % zobrazení grafu
%title('Frequency response of digital filter');
%xlabel('Frequency (Hz)');
%ylabel('Magnitude (dB)');

print('plot_find.bmp', '-dbmp');                                           % uložení grafu

[find_max, find_max_index] = max(find_signal);                             % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', find_max, find_max_index);

%--------------------------------------------------------------------------

fprintf('9. úkol\n');

corr = xcorr(signal, 'biased');

figure;                                                                    % nový graf
fprintf('\t[Graf se zobrazil...]\n');

plot(-samples + 1 : samples - 1, corr);                                    % zobrazení grafu
xlim([-50, 50]);                                                           % limitace vypisu

ref = line([10, 10], ylim);
ref.LineStyle = ':';
ref.Color = [0.4 0.4 0.4];

print('plot_corr.bmp', '-dbmp');                                           % uložení grafu

%--------------------------------------------------------------------------

fprintf('10. úkol\n');

fprintf('\tKoeficient R[10]: %f\n', corr(16010));

%--------------------------------------------------------------------------

fprintf('11. úkol\n');

%--------------------------------------------------------------------------

fprintf('12. úkol\n');

%--------------------------------------------------------------------------

fprintf('13. úkol\n');
