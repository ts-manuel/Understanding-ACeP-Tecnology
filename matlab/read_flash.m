%
% Reads flash content from text file flash.txt
%

clear variables;


% Load flash content into vector
flash = hexfile2vec('flash.txt');

% Extract LUTs
for i = 0:9
    % Color LUTs
    LUT0(i+1,:) = subvect0b(flash, i*2080 +    0, i*2080 +  259);
    LUT1(i+1,:) = subvect0b(flash, i*2080 +  260, i*2080 +  519);
    LUT2(i+1,:) = subvect0b(flash, i*2080 +  520, i*2080 +  779);
    LUT3(i+1,:) = subvect0b(flash, i*2080 +  780, i*2080 + 1039);
    LUT4(i+1,:) = subvect0b(flash, i*2080 + 1040, i*2080 + 1299);
    LUT5(i+1,:) = subvect0b(flash, i*2080 + 1300, i*2080 + 1559);
    LUT6(i+1,:) = subvect0b(flash, i*2080 + 1560, i*2080 + 1819);
    LUT7(i+1,:) = subvect0b(flash, i*2080 + 1820, i*2080 + 2079);
    
    % VCOM LUTs
    LUTV(i+1,:) = subvect0b(flash, i*220 + 20800, i*220 + 21019);
    
    % XON LUTs
    LUTX(i+1,:) = subvect0b(flash, i*200 + 23000, i*200 + 23199);
end

% Temperature boundary
for i = 0:8
    TB(i+1) = subvect0b(flash, i + 25002, i + 25002);
end

% Voltages
for i = 0:9
    VSHC_LVL(i+1) = double(subvect0b(flash, i*2 + 25011, i*2 + 25011));
    VSLC_LVL(i+1) = double(subvect0b(flash, i*2 + 25012, i*2 + 25012));
    LVL2_EN(i+1) = bitget(subvect0b(flash, i + 25031, i + 25031), 7);
    VSLC_LVL2(i+1) = double(bitand(subvect0b(flash, i + 25031, i + 25031), 0x7f));
end

% VCOM DC
VCM_DC = double(subvect0b(flash, 25600, 25600));

% Frame rate
for i = 0:9
    FRAME_RATE(i+1) = subvect0b(flash, 25616, 25616);
end

%--------------------------------------------------------------------------
% Convert raw data
%--------------------------------------------------------------------------

% Temparature boundary [°C]
for i = 1:9
   TB(i) = uint82int8(TB(i)) / 2;   
end

% Voltages [V]
for i = 1:10
   VSHC_LVL(i) = VSHC_LVL(i)*0.2+3;
   VSLC_LVL(i) = -(VSLC_LVL(i)*0.2+3);
   VSLC_LVL2(i) = VSLC_LVL2(i)*0.2+3;
end

VCM_DC = VCM_DC / 20;

% Frame rate [Hz]
for i = 1:10
   switch(FRAME_RATE(i))
       case 0:0x0f
           FRAME_RATE(i) = FRAME_RATE(i)*12.5+12.5;
       case 0x39
           FRAME_RATE(i) = 200;
       case 0x3a
           FRAME_RATE(i) = 100;
       otherwise
           FRAME_RATE(i) = 50;
   end
end

%--------------------------------------------------------------------------
% Compute waveforms
%--------------------------------------------------------------------------
LUTV_W = cell(10);
LUT0_W = cell(10);
LUT1_W = cell(10);
LUT2_W = cell(10);
LUT3_W = cell(10);
LUT4_W = cell(10);
LUT5_W = cell(10);
LUT6_W = cell(10);
LUT7_W = cell(10);
LUTX_W = cell(10);
for i = 1:10
    LUTV_W{i} = lutv2wave(LUTV(i,:), [VCM_DC, 15+VCM_DC, -15+VCM_DC, 0]);
    
    LUT0_W{i} = lutc2wave(LUT0(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT1_W{i} = lutc2wave(LUT1(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT2_W{i} = lutc2wave(LUT2(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT3_W{i} = lutc2wave(LUT3(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT4_W{i} = lutc2wave(LUT4(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT5_W{i} = lutc2wave(LUT5(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT6_W{i} = lutc2wave(LUT6(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);
    LUT7_W{i} = lutc2wave(LUT7(i,:), [0, 15, -15, VSHC_LVL(i), VSLC_LVL(i), VSLC_LVL2(i), 0, 0]);

    LUTX_W{i} = lutx2wave(LUTX(i,:));
end


%--------------------------------------------------------------------------
% Functions
%--------------------------------------------------------------------------

function out = hexfile2vec(file)
% HEXFILE2VEC Loads flash content from text file

    % Load lines from file
    lines = importdata(file);
    out = zeros(1,length(lines)*16);
    
    % Read line by line
    addr = 1;
    for i = 1:length(lines)
        split = strsplit(strip(lines{i}),{' '},'CollapseDelimiters',true);

        for j = 2:length(split)
            out(addr) = hex2dec(split{j});
            addr = addr + 1;
        end
    end
end

function out = subvect0b(vect, start, stop)
    % SUBVECT0b Extracts subvector from vect starting from
    % zero based address start to zero based address stop
    
    out = vect(start+1:stop+1);
end

function out = uint82int8(n)
    isNegative = int8(bitget(n,8));
    out = int8(bitset(n,8,0)) + (-2^7)*isNegative;
end
