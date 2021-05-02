function plot_lut_all(wave, name)
    %PLOT_LUT_ALL Plot waveform for all temperatures
    %   Detailed explanation goes here

    figure();
    hold on;
    for i = 1:length(wave)
        plot(wave{i},'LineWidth',2);
    end
    grid();
    xticks(0:50:700);
    title(name);
    xlabel('Frame number');
    legend('T0','T1','T2','T3','T4','T5','T6','T7','T8','T9');
end

