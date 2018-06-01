function[] = Fuck(unit)

unit = unit.';

figure 
plot([1:length(unit)],unit(1,1:length(unit)),'blue');
hold on 
plot([1:length(unit)],unit(2,1:length(unit)),'red');


ylim([-50 150]);
xlim([0 140]);


figure
plot([1:length(unit)],unit(3,1:length(unit)),'red');

