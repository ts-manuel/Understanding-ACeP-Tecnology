
for T = 0:9
    if T == 0
        tr = strcat(' [Temp < ', int2str(TB(1)), '°C]');
    elseif T == 9
        tr = strcat(' [Temp > ', int2str(TB(9)), '°C]');
    else
        tr = strcat(' [', int2str(TB(T)), '°C < Temp < ', int2str(TB(T+1)), '°C]');
    end
    
    % Plot VCOM waveform - LUT waveform
    plot_lut(LUTV_W{T+1} - LUT0_W{T+1}, strcat('BLACK ', tr), strcat('plots/BLACK_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT1_W{T+1}, strcat('BLUE ', tr), strcat('plots/BLUE_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT2_W{T+1}, strcat('WHITE ', tr), strcat('plots/WHITE_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT3_W{T+1}, strcat('GREEN ', tr), strcat('plots/GREEN_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT4_W{T+1}, strcat('RED ', tr), strcat('plots/RED_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT5_W{T+1}, strcat('YELLOW ', tr), strcat('plots/YELLOW_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT6_W{T+1}, strcat('ORANGE ', tr), strcat('plots/ORANGE_T', int2str(T)));
    plot_lut(LUTV_W{T+1} - LUT7_W{T+1}, strcat('CLEAN ', tr), strcat('plots/CLEAN_T', int2str(T)));
end



