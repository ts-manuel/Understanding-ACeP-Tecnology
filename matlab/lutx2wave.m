function [out] = lutx2wave(lut)
    %LUTX2WAVE Generates waveform for XON LUT
    %   Detailed explanation goes here

    out(1) = 0;
    fn = 1;

    for i = 1:20
        frame = lut((i-1)*10+1:(i-1)*10+10);
        repeat = frame(1);
        
        % Voltage levels
        lv(1) = bitand(bitshift(frame(2), -7), 0x01);
        lv(2) = bitand(bitshift(frame(2), -6), 0x01);
        lv(3) = bitand(bitshift(frame(2), -5), 0x01);
        lv(4) = bitand(bitshift(frame(2), -4), 0x01);
        lv(5) = bitand(bitshift(frame(2), -3), 0x01);
        lv(6) = bitand(bitshift(frame(2), -2), 0x01);
        lv(7) = bitand(bitshift(frame(2), -1), 0x01);
        lv(8) = bitand(bitshift(frame(2), -0), 0x01);
        
        % Generate waveform
        for j = 1:repeat
            for k = 1:8
                cnt = frame(k+2);      
                out(fn:fn+cnt-1) = double(lv(k))*ones(1,cnt);
                fn = fn + cnt;
            end
        end
    end
end

