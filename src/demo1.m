clc;
clear;
close all;

%% dir setup
data_names = ["/home/william/Data/boxes_6dof/text/"
        "/home/william/Data/boxes_rotation/text/"
        "/home/william/Data/boxes_translation/text/"
        "/home/william/Data/calibration/text/"
        "/home/william/Data/dynamic_6dof/text/"
        "/home/william/Data/dynamic_rotation/text/"
        "/home/william/Data/dynamic_translation/text/"
        "/home/william/Data/hdr_boxes/text/"
        "/home/william/Data/hdr_poster/text/"
        "/home/william/Data/office_spiral/text/"
        "/home/william/Data/office_zigzag/text/"
        "/home/william/Data/outdoors_running/text/"
        "/home/william/Data/outdoors_walking/text/"
        "/home/william/Data/poster_6dof/text/"
        "/home/william/Data/poster_rotation/text/"
        "/home/william/Data/poster_translation/text/"
        "/home/william/Data/shapes_6dof/text/"
        "/home/william/Data/shapes_rotation/text/"
        "/home/william/Data/shapes_translation/text/"
        "/home/william/Data/slider_close/text/"
        "/home/william/Data/slider_depth/text/"
        "/home/william/Data/slider_far/text/"
        "/home/william/Data/slider_hdr_close/text/"
        "/home/william/Data/slider_hdr_far/text/"
        "/home/william/Data/urban/text/"];

data_folder = data_names(1);

events_dir = [convertStringsToChars(data_folder), 'events.txt'];
classification_dir = [convertStringsToChars(data_folder), 'classification.txt'];
%% read in events
% ts_hist = [];

fid_events = fopen(events_dir);
fid_classification = fopen(classification_dir);

disp('Reading events and classification ...');

steps = 800;

points = zeros(steps, 2);
class_label = zeros(steps, 1);
count = 0;

event_l = fgetl(fid_events);
class_l = fgetl(fid_classification);

while ischar(event_l) && ischar(class_l)
    count = count + 1;

    event_i = str2num(event_l);
    points(count, :) = event_i(2:3);
    class_i = str2num(class_l);
    class_label(count) = class_i;
    corners = points( class_label == 1,:);

    if count == steps
        % plot
        figure(1), clf;
        plot(points(:, 1), points(:, 2), 'b.');
        hold on;
        plot(corners(:, 1), corners(:, 2), 'ro');
        set(gca, 'YDir','reverse');
        axis equal;
        axis([0 640 0 480])
        drawnow();
        
        % reset
        points = zeros(steps, 2);
        class_label = zeros(steps, 1);
        count = 0;

    end

    event_l = fgetl(fid_events);
    class_l = fgetl(fid_classification);

end

disp('done')

fclose(fid_events);
fclose(fid_classification);
