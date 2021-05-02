function plot_lut(wave, name)
%PLOT_LUT Plot waveform for one temperature
%   Detailed explanation goes here

    figure();
    hold on;
    plot(wave,'LineWidth',2);
    grid();
    xticks(0:50:700);
    title(name);
    xlabel('Frame number');
end

