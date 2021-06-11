clear;
clc;
close all;

%%
data_dir = '/home/william/Data';

dirs = dir(data_dir);
dirs(1:2) = [];

%%
for i = 1:length(dirs)
    i_dir = [dirs(i).folder, '/', dirs(i).name, '/text/'];
    fprintf('%s\n', i_dir);
    %     events_dir = [i_dir, 'events.txt'];
    %     classification_dir = [i_dir, 'classification.txt'];
    %     script_i = sprintf('/home/william/Work/arc_star_ros/arc_star/build/arc_star_app_file %s %s', events_dir, classification_dir);
    %     [status, cmdout] = system(script_i);
    %     fprintf('Done. \n');
end
