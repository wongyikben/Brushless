function[] = PoleFuck(pole)
func = [1];
for i=1:length(pole)
    func = conv(func,[1 -pole(i)]);
end

ans = inv([0.4294 0 0 -func(1);1.321 0.4294 0 -func(2);0 1.321 0.4294 -func(3);0 0 1.321 -func(4)])*[-1;-0.6938;-2.097;0];

P = ans(2)*1024

I = ans(3)*1024

D = ans(1)*1024