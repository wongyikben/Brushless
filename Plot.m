function []= Plot()

s = serial('COM5','BAUD',115200);
try
    fopen(s);
catch err
    fclose(instrfind);
    error('NOT CONNECTED.');
end

Ts = 0.01;
Max = 10/0.01;
t = (0.01:0.01:Max*0.01);
input = zeros(1,Max);
output = zeros(1,Max);
last_time = 0;
index = 1;

figure
plot(t,input,'blue');
hold on
plot(t,output,'red');

tic
while(1)
   time = uint32(toc/Ts); 
   if (time~=last_time)
       
       if(index>500)
        index = 1;
        input = zeros(1,Max);
        output = zeros(1,Max);
       end
     
       last_time = time;
       buffer = fgetl(s);
       buffer = strsplit(buffer);
       input(index) = str2double(buffer(1));
       output(index) = str2double(buffer(2));
       drawnow 
       index = index + 1;
   end    
end
