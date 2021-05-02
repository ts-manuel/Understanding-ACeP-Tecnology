%
% Print flash content to file
%
    
fileID = fopen('content.txt', 'w');

% Print VCOM DC
fprintf(fileID, "------------------------------------------\n");
fprintf(fileID, " VCOM DC OFFSET SAME FOR ALL TEMPERATURES \n");
fprintf(fileID, "------------------------------------------\n");
fprintf(fileID, "VCM_DC = %d V\n",  VCM_DC);
fprintf(fileID, "\n");

% Print voltages - framerates
fprintf(fileID, "---------------------------------------------------\n");
fprintf(fileID, " VOLTAGES AND FRAMERATE FOR DIFFERENT TEMPERATURES \n");
fprintf(fileID, "---------------------------------------------------\n");
for i = 0:9
    if i == 0
        fprintf(fileID, "T0 TEMP < %d°C\n",  TB(i+1));
    elseif i == 9
        fprintf(fileID, "T0 TEMP > %d°C\n",  TB(i));
    else
        fprintf(fileID, "T%d %d°C <= TEMP < %d°C\n", i, TB(i), TB(i+1));
    end

    fprintf(fileID, "VSHC_LVL   = %4d V\n", VSHC_LVL(i+1));
    fprintf(fileID, "VSLC_LVL   = %4d V\n", VSLC_LVL(i+1));
    fprintf(fileID, "LVL2_EN    = %4d\n", LVL2_EN(i+1));
    fprintf(fileID, "VSLC_LVL2  = %4d V\n", VSLC_LVL2(i+1));
    fprintf(fileID, "FRAME RATE = %4d Hz\n", FRAME_RATE(i+1));
    
    fprintf(fileID, "\n");
end

% LUTs
for i = 0:9
    fprintf(fileID, "---------------------------------\n");
    if i == 0
        fprintf(fileID, " T0 LUTs TEMP < %d°C\n",  TB(i+1));
    elseif i == 9
        fprintf(fileID, " T0 LUTs TEMP > %d°C\n",  TB(i));
    else
        fprintf(fileID, " T%d LUTs %d°C <= TEMP < %d°C\n", i, TB(i), TB(i+1));
    end
    fprintf(fileID, "---------------------------------\n");
    
    print_array(fileID, LUTV(i+1,:), 'LUT_VCOM', 11, 20);
    print_array(fileID, LUT0(i+1,:), 'LUT_COLOR_0', 13, 20);
    print_array(fileID, LUT1(i+1,:), 'LUT_COLOR_1', 13, 20);
    print_array(fileID, LUT2(i+1,:), 'LUT_COLOR_2', 13, 20);
    print_array(fileID, LUT3(i+1,:), 'LUT_COLOR_3', 13, 20);
    print_array(fileID, LUT4(i+1,:), 'LUT_COLOR_4', 13, 20);
    print_array(fileID, LUT5(i+1,:), 'LUT_COLOR_5', 13, 20);
    print_array(fileID, LUT6(i+1,:), 'LUT_COLOR_6', 13, 20);
    print_array(fileID, LUT7(i+1,:), 'LUT_COLOR_7', 13, 20);
    print_array(fileID, LUTX(i+1,:), 'LUT_XON', 10, 20);

    fprintf(fileID, "\n");
end


function print_array(fileID, lut, name, w, h)
    fprintf(fileID, "uint8_t %s[] = {\n", name);
    
    addr = 1;
    for i = 1:h
        fprintf(fileID, "\t");
        
        for j = 1:w
            fprintf(fileID, "%3d", lut(addr));
            if j ~= w; fprintf(fileID, ","); end
            addr = addr+1;
        end
        
        if i == h
            fprintf(fileID, "\n");
        else
            fprintf(fileID, ",\n");
        end
    end
    
    fprintf(fileID, "};\n\n");
end