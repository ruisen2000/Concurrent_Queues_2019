one = [3414349 2888732 2265208 1813698];
two = [2469686 1864317 3363856 2938988];
non = [5432255 3957708 3962006 3855897];
x = 1:4


hold on
plot(x,one, '-o');
plot(x,two, '-o');
plot(x,non, '-o');
xlabel('Number of Producers and Consumers') 
ylabel('Throughput') 
legend({'One-Lock Queue','Two-Lock Queue','Non-Blocking Queue'},'Location','northeast')
xticks([1 2 3 4])