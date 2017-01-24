#program step(t).
#heuristic holds(X,V,t-1) : holds(X,V,t).     [10000-t+1,true]
%#heuristic holds(X,V,t-1) : not holds(X,V,t). [10000-t+1,false]
