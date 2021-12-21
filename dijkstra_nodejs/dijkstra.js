const GRAPH_TO_GO = Object.freeze(
    {
        start: { A: 5, B: 2 },
        A: { C: 4, D: 2 },
        B: { A: 8, D: 7 },
        C: { D: 6, finish: 3 },
        D: { finish: 1 },
        finish: {}
    }
);

const START_POSITION_NODE = "start";
const FINISH_POSITION_NODE = "finish";

const lowestCostNode = (costs, processed) => {
    return Object.keys(costs).reduce((lowest, node) => {
        if (lowest === null || costs[node] < costs[lowest]) {
            if (!processed.includes(node)) {
                lowest = node;
            }
        }
        return lowest;
    }, null);
};

const dijkstra = (graph) => {
    const costs = Object.assign({ finish: Infinity }, graph.start);
    const parents = { finish: null };
    for (let child in graph.start) {
        parents[child] = START_POSITION_NODE;
    }
    const processed = [];
    let node = lowestCostNode(costs, processed);
    while (node) {
        let cost = costs[node];
        let children = graph[node];
        Object.keys(children).forEach(n => {
            const newCost = cost + children[n];
            if (!costs[n] || costs[n] > newCost) {
                costs[n] = newCost;
                parents[n] = node;
            }
        })
        processed.push(node);
        node = lowestCostNode(costs, processed);
    }
    let optimalPath = [FINISH_POSITION_NODE];
    let parent = parents.finish;
    while (parent) {
        optimalPath.push(parent);
        parent = parents[parent];
    }
    optimalPath.reverse();

    const results = {
        distance: costs.finish,
        path: optimalPath
    };

    return results;
};

console.log(dijkstra(GRAPH_TO_GO));