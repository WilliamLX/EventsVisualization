clc;
clear;
close all;

%% dir setup
% events_dir = '../result/events.txt';

events_dir = '/home/william/Data/boxes_6dof/text/events.txt';
% events_dir = '/home/william/Data/slider_depth/text/events.txt';

classification_dir = '../result/classification.txt';


%% read in events
events = readmatrix(events_dir);
classification = readmatrix(classification_dir);

%% plot
events_num = size(events, 1);
window_size = 100000;

steps = ceil(events_num / window_size);

figure(1);
hold on;

for i = 1:steps
    i_start = (i - 1) * window_size +1;
    i_end = i * window_size;
    points = events(i_start:i_end, [2, 3]);
    corners = points(logical(classification(i_start:i_end)), :);
    figure(1);
    clf;
%     plot(points(:, 1), 480 - points(:, 2), 'b.');
%     hold on;
    plot(corners(:, 1), 480 - corners(:, 2), 'r.');
    axis([0 640 0 480])
    drawnow();
end
% 
% %
% % points = events(1:window_size, [2,3]);
% % corners = points(logical(classification(1:window_size)),:);
% %
% %
% % figure(1);
% % hold on;
% %
% % plot(points(:,1), 480-points(:,2), 'b.')
% % plot(corners(:,1), 480-corners(:,2), 'r.')
% 
% %
% %
% % points = events(1:window_size, [2,3]);
% % corners = points(logical(classification(1:window_size)),:);
% %
% %
% % points = events(1:window_size, [2,3]);
% % corners = points(logical(classification(1:window_size)),:);
% %
% %
% % points = events(1:window_size, [2,3]);
% % corners = points(logical(classification(1:window_size)),:);
% 
% % for i = 1:events_num
% %     fprintf('%i\n', i);
% %
% %     if mod(i,window_size) == 0
% %
% %     end
% % end
