clc;
clear;
close all;


%% dir setup
events_dir = '../result/events.txt';
classification_dir = '../result/classification.txt';

%% read in events
fid = fopen(events_dir);
events = cell2mat(textscan(fid,'%f%f%f%f'));

%% read in classification
fid = fopen(classification_dir);
classification = cell2mat(textscan(fid,'%f'));

%% plot
events_num = size(events,1);
window_size = 5000;


points = events(1:window_size, [2,3]);
corners = points(logical(classification(1:window_size)),:);


figure(1);
hold on;

plot(points(:,1), points(:,2), 'b.')
plot(corners(:,1), corners(:,2), 'r.')

% 
% 
% points = events(1:window_size, [2,3]);
% corners = points(logical(classification(1:window_size)),:);
% 
% 
% points = events(1:window_size, [2,3]);
% corners = points(logical(classification(1:window_size)),:);
% 
% 
% points = events(1:window_size, [2,3]);
% corners = points(logical(classification(1:window_size)),:);




% for i = 1:events_num
%     fprintf('%i\n', i);
%     
%     if mod(i,window_size) == 0
%         
%     end
% end

