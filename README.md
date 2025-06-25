# CMPT 383: Comparative programming languages
This was to demonstrate the use of multiple programming languages working together in a single project.

## About

Graph Calculator is a server/app that allows you to easily create weighted or
unweighted graphs and run different algorithms on them to view the results. The
graph creation happens in your browser, and the actual algorithms are performed
on the server side with c++. The idea is that this can be scaled up to large
graphs where some operations can be fairly time consuming, and you may not want
them to be done on the users side.

The server itself is meant to run on a linux system, however the app. itself 
runs on a browser. So it should run on any platform with a web browser and access
to the server through a network.

Information is passed from the user to the server using JSON strings, and the
server converts those and hands them off to the c++ program.

The idea behind this is that it can be optimized to improve performance, more 
algorithms can be added and it can then be used for solving any problem that 
can be reduced to a graphing problem. With the way its set up, other 
applications could in theory turn their problem into a graph and request 
processing here. An example might be using a max flow algorithm to try and
estimate the most efficient use of resources in a production line.

## Dependancies

### For virtual machine
- virtualbox
- vagrant

### For server
- Python3
- pip3
- venv
- Flask
- c++
- cmake

## Building
Just run the usual vagrant things it should build.

## Using the app
SSH into vagrant and enter the command and the server should start.
```bash
$ cd project/server/ && sudo python3 flaskServer.py
```
don't forget to delete the '$' at the front.

Once the server is running you should be able to access it from your browser at
http://localhost:8080

## Available Algorithms and How to Use Them
#### Prims Algorithm
Finds a minimum spanning tree in a graph.
Required conditions of your graph.
- Undirected
- Positive
- Connected
- More than 1 vertex

#### Breadth First Search (BFS)
Finds a spanning tree in a graph.
Requred conditions of your graph.
- Positive
- Connected
- More than 1 vertex

#### Dreadth First Search (DFS)
Finds a spanning tree in a graph.
Requred conditions of your graph.
- Positive
- Connected
- More than 1 vertex

## Extra (please read)
I am not a web developer, and I very rarely handle GUIs. I decided this could
be an opportunity to try some new things I am not generally comfortable with.
But because of this, the page is not very pretty and I don't know any web
frameworks or systems. Most of the UI was made using basic javascript because
there is just far to much extra stuff added on top of these things for web dev.
that I am not familiar with.
