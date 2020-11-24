#!/usr/local/bin/zsh
for i in {1..48..1}
do
    build/isid --data=/home/a77763t/dissertation/playground/datasets/canidae.tsv --threads=$i --matrix=/home/a77763t/dissertation/playground/datasets/dist_canidae.csv
    build/isid --data=/home/a77763t/dissertation/playground/datasets/aves.tsv --threads=$i --matrix=/home/a77763t/dissertation/playground/datasets/dist_aves.csv
    build/isid --data=/home/a77763t/dissertation/playground/datasets/culicidae.tsv --threads=$i  --matrix=/home/a77763t/dissertation/playground/datasets/dist_culicidae.csv
done