function X = plotRun(filename)
  X = dlmread(filename, ';');
  plot(X(:,1), X(:,2), 'o');
endfunction
