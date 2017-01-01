% ISS Projekt by Jakub Fr�z (xfryzj01)

clc;            % clear command window
clear;          % clear workspace
close all;      % close all figures

%--------------------------------------------------------------------------

fprintf('1. �kol\n');

[signal, fs] = audioread('xfryzj01.wav');                                  % na�ten� sign�lu

fprintf('\tVzorkovac� frekvence Fs = %d kHz\n', fs/1000);                  % vzorkovac� frekvence

samples = numel(signal);
fprintf('\tD�lka sign�lu: %d s (%d vzork�)\n', samples/fs, samples);       % d�lka ve vzorc�ch

%--------------------------------------------------------------------------

fprintf('2. �kol\n');

dft = fft(signal);                                                         % DFT nad na�ten�m sign�lem
dft_abs = abs(dft);

figure('Name', 'Discrete Fourier Transform plot of signal');               % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

plot(dft_abs(1:samples/2));                                                % zobrazen� grafu
title('Discrete Fourier Transform plot of signal');
xlabel('Frequency [Hz]');
ylabel('Magnitude');

print('plot_dft.bmp', '-dbmp');                                            % ulo�en� grafu

%--------------------------------------------------------------------------

fprintf('3. �kol\n');

[dft_max, dft_max_index] = max(dft_abs);                                   % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', dft_max, dft_max_index);

%--------------------------------------------------------------------------

fprintf('4. �kol\n');

b = [0.2324, -0.4112, 0.2324];                                             % b koeficienty
a = [1, 0.2289, 0.4662];                                                   % a koeficienty

[z, p, k] = tf2zpk(b,a);                                                   % nuly, p�ly, gain

figure('Name', 'Zero/Pole plot of IIR filter');                            % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

zplane(z, p);                                                              % zobrazen� grafu
title('Zero/Pole plot of IIR filter');
xlabel('Real Part');
ylabel('Imaginary Part');
legend('Zero', 'Pole');

print('plot_zp.bmp', '-dbmp');                                             % ulo�en� grafu

if isstable(b, a) == true                                                  % stabilita
    fprintf('\tFiltr je stabiln�\n');
else
    fprintf('\tFiltr nen� stabiln�\n');
end

%--------------------------------------------------------------------------

fprintf('5. �kol\n');

[h, w] = freqz(b, a, fs/2);                                                % kmito�tov� charakteristika filtru

figure('Name', 'Frequency response of digital filter');                    % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

plot((w*fs)/(2*pi), abs(h));                                               % zobrazen� grafu
ylim([0 1])
title('Frequency response of digital filter');
xlabel('Frequency (Hz)');
ylabel('Magnitude');

print('plot_iir.bmp', '-dbmp');                                            % ulo�en� grafu

%--------------------------------------------------------------------------

fprintf('6. �kol\n');

signalf = filter(b, a, signal);                                            % filtrovan� sign�l
dftf = fft(signalf);                                                       % DFT nad filtrovan�m sign�lem
dftf_abs = abs(dftf);

figure('Name', 'Discrete Fourier Transform plot of filtered signal');      % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

plot(dftf_abs(1:samples/2));                                               % zobrazen� grafu
title('Discrete Fourier Transform plot of filtered signal');
xlabel('Frequency [Hz]');
ylabel('Magnitude');

print('plot_dftf.bmp', '-dbmp');                                           % ulo�en� grafu

%--------------------------------------------------------------------------

fprintf('7. �kol\n');

[dftf_max, dftf_max_index] = max(abs(dftf));                               % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', dftf_max, dftf_max_index);

%--------------------------------------------------------------------------

fprintf('8. �kol\n');

find_filter = repmat([1, 1, 1, 1, -1, -1, -1, -1], 1, 40);                 % 40 sekvenc� [h h h h -h -h -h -h]

find_signal = filter(find_filter, 1, signal);

figure;                                                                    % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

plot(find_signal);                                                         % zobrazen� grafu
%title('Frequency response of digital filter');
%xlabel('Frequency (Hz)');
%ylabel('Magnitude (dB)');

print('plot_find.bmp', '-dbmp');                                           % ulo�en� grafu

[find_max, find_max_index] = max(find_signal);                             % maximum a index maxima
fprintf('\tMaximum spektra: amplituda %.2f frekvence %d Hz\n', find_max, find_max_index);

%--------------------------------------------------------------------------

fprintf('9. �kol\n');

corr = xcorr(signal, 'biased');

figure;                                                                    % nov� graf
fprintf('\t[Graf se zobrazil...]\n');

plot(-samples + 1 : samples - 1, corr);                                    % zobrazen� grafu
xlim([-50, 50]);                                                           % limitace vypisu

ref = line([10, 10], ylim);
ref.LineStyle = ':';
ref.Color = [0.4 0.4 0.4];

print('plot_corr.bmp', '-dbmp');                                           % ulo�en� grafu

%--------------------------------------------------------------------------

fprintf('10. �kol\n');

fprintf('\tKoeficient R[10]: %f\n', corr(16010));

%--------------------------------------------------------------------------

fprintf('11. �kol\n');

%--------------------------------------------------------------------------

fprintf('12. �kol\n');

%--------------------------------------------------------------------------

fprintf('13. �kol\n');
