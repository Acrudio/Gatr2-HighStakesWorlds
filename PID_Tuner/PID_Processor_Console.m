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

% starting from your filtered string array:
lines = splitlines(csvText);                 
keep  = startsWith(lines, "[PID]");    
filtered = lines(keep);                

% Option A: simple erase (if every tag is exactly "[PID] ")
csvText = erase(filtered, "[PID]");  
csvText  = strjoin(csvText, newline);  % stitch back into one big string


lines = splitlines(csvText);        % gives you a string array of each line
rest  = lines(3:end);         % everything but the first two
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
yline(str2double(targetValue), "b-.");
xlabel("Time [ms]");
ylabel("Tracking Value [?]")
legend("Tracking Data", sprintf("Target (%s)", targetValue))


% Set title to the PID constants from first line of csv
titleLine = lines(1);
titleText = sprintf("%s",titleLine{1}(3:length(titleLine{1})));
title(titleText,'Interpreter', 'none');
fprintf("\nLoaded succesfully!")