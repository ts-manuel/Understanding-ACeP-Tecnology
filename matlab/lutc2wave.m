function [out] = lutc2wave(lut, volt)
    %LUTV2WAVE Generates waveform for COLOR LUT
    %   Detailed explanation goes here

    out(1) = 0;
    fn = 1;

    for i = 1:20
        frame = lut((i-1)*13+1:(i-1)*13+13);
        repeat = frame(1);
        
        % Voltage levels
        lv(1) = bitand(bitshift(frame(2), -4), 0x07);
        lv(2) = bitand(bitshift(frame(2), -0), 0x07);
        lv(3) = bitand(bitshift(frame(3), -4), 0x07);
        lv(4) = bitand(bitshift(frame(3), -0), 0x07);
        lv(5) = bitand(bitshift(frame(4), -4), 0x07);
        lv(6) = bitand(bitshift(frame(4), -0), 0x07);
        lv(7) = bitand(bitshift(frame(5), -4), 0x07);
        lv(8) = bitand(bitshift(frame(5), -0), 0x07);
        
        % Generate waveform
        for j = 1:repeat
            for k = 1:8
                cnt = frame(k+5);      
                out(fn:fn+cnt-1) = volt(lv(k)+1)*ones(1,cnt);
                fn = fn + cnt;
            end
        end
    end
end

