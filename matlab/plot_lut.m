function plot_lut(wave, name, file)
%PLOT_LUT Plot waveform for one temperature
%   Detailed explanation goes here

    if exist('file', 'var')
        fig = figure('Visible','off');
    else
        fig = figure();
    end
    
    fig.Position = [0 0 2000 500]; 
    hold on;
    plot(wave,'LineWidth',2);
    grid();
    xticks(0:50:700);
    title(name);
    xlabel('Frame number');
    
    if exist('file', 'var')
        print(fig,'-dpng', file);
        close(fig);
    end
end

% function plot_lut(wave, name, file)
% %PLOT_LUT Plot waveform for one temperature
% %   Detailed explanation goes here
% 
%     figure();
%     hold on;
%     plot(wave,'LineWidth',2);
%     grid();
%     xticks(0:50:700);
%     title(name);
%     xlabel('Frame number');
% end

