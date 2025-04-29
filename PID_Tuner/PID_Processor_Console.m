clear; clc;
% Constants
% folderName = "D:\";

% Getting the folder data
% folderData= dir(folderName);
% mask = [folderData.isdir] == false;
% folderData = folderData(mask);

% Getting the file
% for k = 1:length(folderData)
%     fprintf("[%d] %s\n", k, folderData(k).name); % Display file names
% end
% choice = input("Enter Index to Read: ");
% fileName = fullfile(folderName,folderData(choice).name);
% fprintf("Loading %s...",fileName)

% Reading data

% 1. Grab CSV from clipboard (or from your input loop)
csvText = clipboard('paste');
rawLines = splitlines(csvText);

% starting from your filtered string array:
lines = splitlines(csvText);                 
keep  = startsWith(lines, "[PID]");    
filtered = lines(keep);                

% Option A: simple erase (if every tag is exactly "[PID] ")
csvText = erase(filtered, "[PID]");  
csvText  = strjoin(csvText, newline);  % stitch back into one big string

dataStartLine = 3;

lines = splitlines(csvText);        % gives you a string array of each line
rest  = lines(dataStartLine:end);         % everything but the first two
csvText  = strjoin(rest, newline);  % stitch back into one big string

% 2. Write it out to a temp .csv
fname = 'temp.csv';
fid = fopen(fname,'w');
if fid<0, error('Couldn''t open temp file'); end

thingy = strsplit(csvText,newline);
fprintf(fid, '%s\n', thingy{:});
fclose(fid);

% % 3. Read it in properly
% T = readtable(fname, ...
%     'Delimiter', ',', ...
%     'ReadVariableNames', true ...
% );

% read it straight into a table
data = readtable( ...
  fname, ...         % supply the CSV as a string
  'Delimiter', ',', ...        % comma-separated
  'ReadVariableNames', true ...% first row is headers
);


% Plot
figure;
hold on;
plot(data.Time, data.TrackingValue, "r")

% Extract target data from second line of csv

targetLine = lines(2);
targetValue = targetLine{1}(3:length(targetLine{1}));
targetValue = str2double(targetValue);
yline(targetValue, "b");
xlabel("Time [ms]");
ylabel("Tracking Value [?]")
legend("Tracking Data", sprintf("Target (%.1f)", targetValue))


% Set title to the PID constants from first line of csv
titleLine = lines(1);
titleText = sprintf("%s",titleLine{1}(3:length(titleLine{1})));
title(titleText,'Interpreter', 'none');
fprintf("\nLoaded succesfully!")

% % XY Exit Detection
% % Automatic Exit Detection
% mask = contains(rawLines, "XY: "); % find the line containing the exit 
% foundExitLine = rawLines(mask); % Mask out the line containing the exit
% B = circshift(mask, -1); % Shift the mask back to get the time line before this
% timeContainingLine = rawLines(B); % Pull out just the time containing line
% % Extract the exit time
% numStr = regexp(timeContainingLine, '[-+]?\d*\.?\d+', 'match', 'once'); % Take the exit time from that line
% XY_ExitTime = str2double(numStr);

[XY_ExitTime, XY_ExitMode] = FindExitTime(rawLines, "XY: ");
xline(XY_ExitTime,"g","DisplayName",sprintf("XY Exit (%.1f) - %s", XY_ExitTime,XY_ExitMode{1})); % graph the line

[Angle_ExitTime, Angle_ExitMode] = FindExitTime(rawLines, "Angle: ");
xline(Angle_ExitTime,"k-.","DisplayName",sprintf("Angle Exit (%.1f) - %s", Angle_ExitTime, Angle_ExitMode{1})); % graph the line

smallExit = input("\nExit Range: ");
yline(targetValue + smallExit, "b-.", "DisplayName",sprintf("Upper Range Gate (+ %.3f)", smallExit))
yline(targetValue - smallExit, "c-.", "DisplayName",sprintf("Lower Range Gate (- %.3f)", smallExit))

