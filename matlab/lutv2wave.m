function [out] = lutv2wave(lut, volt)
    %LUTV2WAVE Generates waveform for VCOM LUT
    %   Detailed explanation goes here

    out(1) = 0;
    fn = 1;

    for i = 1:20
        frame = lut((i-1)*11+1:(i-1)*11+11);
        repeat = frame(1);
        
        % Voltage levels
        lv(1) = bitand(bitshift(frame(2), -6), 0x03);
        lv(2) = bitand(bitshift(frame(2), -4), 0x03);
        lv(3) = bitand(bitshift(frame(2), -2), 0x03);
        lv(4) = bitand(bitshift(frame(2), -0), 0x03);
        lv(5) = bitand(bitshift(frame(3), -6), 0x03);
        lv(6) = bitand(bitshift(frame(3), -4), 0x03);
        lv(7) = bitand(bitshift(frame(3), -2), 0x03);
        lv(8) = bitand(bitshift(frame(3), -0), 0x03);
        
        % Generate waveform
        for j = 1:repeat
            for k = 1:8
                cnt = frame(k+3);      
                out(fn:fn+cnt-1) = volt(lv(k)+1)*ones(1,cnt);
                fn = fn + cnt;
            end
        end
    end
end

