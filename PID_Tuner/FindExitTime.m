function [ExitTime, ExitMode] = FindExitTime(rawLines,KeyText)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

% XY Exit Detection
% Automatic Exit Detection
mask = contains(rawLines, KeyText); % find the line containing the exit 
foundExitLine = rawLines(mask); % Mask out the line containing the exit
B = circshift(mask, -1); % Shift the mask back to get the time line before this
timeContainingLine = rawLines(B); % Pull out just the time containing line
% Extract the exit time
numStr = regexp(timeContainingLine, '[-+]?\d*\.?\d+', 'match', 'once'); % Take the exit time from that line
ExitTime = str2double(numStr);

ExitMode = extractBetween(foundExitLine, KeyText, ",");
end