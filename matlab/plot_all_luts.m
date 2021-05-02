close all;
clear vcom_wave
clear lut0_wave lut1_wave lut2_wave lut3_wave lut4_wave lut5_wave lut6_wave lut7_wave
clear xon_wave

vcom_wave = cell(10);
lut0_wave = cell(10);
lut1_wave = cell(10);
lut2_wave = cell(10);
lut3_wave = cell(10);
lut4_wave = cell(10);
lut5_wave = cell(10);
lut6_wave = cell(10);
lut7_wave = cell(10);
xon_wave = cell(10);
for i = 1:10
    vcom_wave{i} = lutv2wave(LUTV(i,:), [VCM_DC, 15+VCM_DC, -15+VCM_DC, 0]);
    
    lut0_wave{i} = lutc2wave(LUT0(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut1_wave{i} = lutc2wave(LUT1(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut2_wave{i} = lutc2wave(LUT2(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut3_wave{i} = lutc2wave(LUT3(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut4_wave{i} = lutc2wave(LUT4(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut5_wave{i} = lutc2wave(LUT5(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut6_wave{i} = lutc2wave(LUT6(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    lut7_wave{i} = lutc2wave(LUT7(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);

    xon_wave{i} = lutx2wave(LUTX(i,:));
end

% Plot waveforms
% plot_lut_all(vcom_wave, 'VCOM');
% plot_lut_all(lut0_wave, 'LUT0 (BLACK)');
% plot_lut_all(lut1_wave, 'LUT1 (WHITE)');
% plot_lut_all(lut2_wave, 'LUT2 (GREEN)');
% plot_lut_all(lut3_wave, 'LUT3 (BLUE)');
% plot_lut_all(lut4_wave, 'LUT4 (RED)');
% plot_lut_all(lut5_wave, 'LUT5 (YELLOW)');
% plot_lut_all(lut6_wave, 'LUT6 (ORANGE)');
% plot_lut_all(lut7_wave, 'LUT7 (CLEAN)');
% plot_lut_all(xon_wave, 'XON');

T = 2;
plot_lut(vcom_wave{T+1}, 'VCOM');
plot_lut(lut0_wave{T+1}, 'LUT0 (BLACK)');
plot_lut(lut1_wave{T+1}, 'LUT1 (WHITE)');
plot_lut(lut2_wave{T+1}, 'LUT2 (GREEN)');
plot_lut(lut3_wave{T+1}, 'LUT3 (BLUE)');
plot_lut(lut4_wave{T+1}, 'LUT4 (RED)');
plot_lut(lut5_wave{T+1}, 'LUT5 (YELLOW)');
plot_lut(lut6_wave{T+1}, 'LUT6 (ORANGE)');
plot_lut(lut7_wave{T+1}, 'LUT7 (CLEAN)');
plot_lut(xon_wave{T+1}, 'XON');
