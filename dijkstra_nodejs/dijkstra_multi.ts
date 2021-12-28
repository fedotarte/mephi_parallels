import { Worker, isMainThread, parentPort } from 'worker_threads';

class VertexNode {
    nameOfVertex: string;
    weight: number;
}

class Vertex {
    name: string;
    nodes: VertexNode[];
    weight: number;

    constructor(theName: string, theNodes: VertexNode[], theWeight: number = 1) {
        this.name = theName;
        this.nodes = theNodes;
        this.weight = theWeight;
    }
}

class Dijkstra {

    vertices: any;
    constructor() {
        this.vertices = {};
    }

    addVertex(vertex: Vertex): void {
        this.vertices[vertex.name] = vertex;
    }

    setPathToNodes(nodeName: string, weight: number):VertexNode {
        return {nameOfVertex:nodeName.toUpperCase(), weight}
    }

    findPointsOfShortestWay(start: string, finish: string, weight: number): string[] {

        let nextVertex: string = finish;
        let arrayWithVertex: string[] = [];
        while (nextVertex !== start) {

            let minWeigth: number = Number.MAX_VALUE;
            let minVertex: string = "";
            for (let i of this.vertices[nextVertex].nodes) {
                if (i.weight + this.vertices[i.nameOfVertex].weight < minWeigth) {
                    minWeigth = this.vertices[i.nameOfVertex].weight;
                    minVertex = i.nameOfVertex;
                }
            }
            arrayWithVertex.push(minVertex);
            nextVertex = minVertex;
        }
        return arrayWithVertex;
    }


    findShortestWay(start: string, finish: string): string[] {
        let arrayWithVertex;
        if (isMainThread) {
            const worker = new Worker(__filename);
          // Receive messages from the worker thread
            worker.once('message', (message) => {
                let nodes: any = {};
                let visitedVertex: string[] = [];
        
                for (let i in this.vertices) {
                    if (this.vertices[i].name === start) {
                        this.vertices[i].weight = 0;
        
                    } else {
                        this.vertices[i].weight = Number.MAX_VALUE;
                    }
                    nodes[this.vertices[i].name] = this.vertices[i].weight;
                }
        
                while (Object.keys(nodes).length !== 0) {
                    let sortedVisitedByWeight: string[] = Object.keys(nodes).sort((a, b) => this.vertices[a].weight - this.vertices[b].weight);
                    let currentVertex: Vertex = this.vertices[sortedVisitedByWeight[0]];
                    for (let j of currentVertex.nodes) {
                        const calculateWeight: number = currentVertex.weight + j.weight;
                        if (calculateWeight < this.vertices[j.nameOfVertex].weight) {
                            this.vertices[j.nameOfVertex].weight = calculateWeight;
                        }
                    }
                    delete nodes[sortedVisitedByWeight[0]];
                }
                const finishWeight: number = this.vertices[finish].weight;
                let arrayWithVertex: string[] = this.findPointsOfShortestWay(start, finish, finishWeight).reverse();
                arrayWithVertex.push(finish, finishWeight.toString());
                return arrayWithVertex;
              console.log(message + ' received path the worker thread!');
            });
            worker.postMessage('got way');
          } else {
            parentPort.once('message', (message) => {
              console.log(message + ' received from the parent thread with way');
              parentPort.postMessage('pong');
            });
        }
        return arrayWithVertex;
    }
}

const graph = new Dijkstra();
graph.addVertex(new Vertex(
    "A", 
    [
        graph.setPathToNodes("B", 3), 
        graph.setPathToNodes("C", 5)
    ]
    ));
graph.addVertex(new Vertex(
    "B", [
        graph.setPathToNodes("A", 3),
        graph.setPathToNodes("C", 2), 
       graph.setPathToNodes("D", 6)
    ]));
graph.addVertex(new Vertex(
    "C", 
    [
        graph.setPathToNodes("B", 2),
        graph.setPathToNodes("A", 5),
        graph.setPathToNodes("d", 5)
    ]));
graph.addVertex(new Vertex(
    "D", 
    [
    graph.setPathToNodes("B",6),
    graph.setPathToNodes("C", 5)
    ]));

/**
 * for A, D
 * expected [ 'A', 'B', 'D', '9' ]
 */

console.log(graph.findShortestWay("A", "D"));