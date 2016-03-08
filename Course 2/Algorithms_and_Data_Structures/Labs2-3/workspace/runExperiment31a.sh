#!/bin/bash 

if [ ! -d "./timestamps" ]; then
	mkdir timestamps
	mkdir timestamps/dijkstra
	mkdir timestamps/bellmanford
fi

#-------------- 3.1.a ---------------#
# verticesAmount: iterable
# edgesAmount: (verticesAmount^2)/10
# minWeight: 1; maxWeight: 10^6

tput setaf 1; 
echo "Experiment 3.1.a"
tput sgr0 

minWeight=1
maxWeight=1000000

> timestamps/dijkstra/dijkstra-31a.txt
> timestamps/bellmanford/bellmanford-31a.txt

for verticesAmount in {1..10001..100}
do
	edgesAmount=$[verticesAmount^2/10]

	SECONDS=0 # built in bash constant which auto-increments
	./GraphAlgorithms run --algorithm dijkstra --directed 1 -va $verticesAmount -ea $edgesAmount -min $minWeight -max $maxWeight --start 0 --finish $[verticesAmount-1] >> timestamps/dijkstra/dijkstra-31a.txt
	printf "vertices amount: %s\texecution time: %s\n" "$verticesAmount" "$SECONDS" >> timestamps/dijkstra/dijkstra-31a.txt

	SECONDS=0 
	./GraphAlgorithms run --algorithm bellman-ford --directed 1 -va $verticesAmount -ea $edgesAmount -min $minWeight -max $maxWeight --start 0 --finish $[verticesAmount-1] >> timestamps/bellmanford/bellmanford-31a.txt
	printf "vertices amount: %s\texecution time: %s\n" "$verticesAmount" "$SECONDS" >> timestamps/bellmanford/bellmanford-31a.txt
done
