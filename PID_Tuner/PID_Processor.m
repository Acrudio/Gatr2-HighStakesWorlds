clear; clc;
% Constants
folderName = "D:\";

% Getting the folder data
folderData= dir(folderName);
mask = [folderData.isdir] == false;
folderData = folderData(mask);

% Getting the file
for k = 1:length(folderData)
    fprintf("[%d] %s\n", k, folderData(k).name); % Display file names
end
choice = input("Enter Index to Read: ");
fileName = fullfile(folderName,folderData(choice).name);
fprintf("Loading %s...",fileName)

% Reading data
lines = readlines(fileName); % read full file into lines
data = readtable(fileName); % read just data

% Plot
figure;
hold on;
plot(data.Time, data.TrackingValue, "r--")

% Extract target data from second line of csv
targetLine = lines(2);
targetValue = targetLine{1}(3:length(targetLine{1}));
yline(str2double(targetValue), "b-.");
xlabel("Time [ms]");
ylabel("Tracking Value [?]")
legend("Tracking Data", sprintf("Target (%s)", targetValue))


% Set title to the PID constants from first line of csv
titleLine = lines(1);
titleText = sprintf("%s \n %s",fileName,titleLine{1}(3:length(titleLine{1})));
title(titleText,'Interpreter', 'none');
fprintf("\nLoaded succesfully!")