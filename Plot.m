[] function = Plot()

s = serial('COM5','BAUD',115200);
try
    fopen(s);
catch err
    fclose(instrfind);
    error('NOT CONNECTED.');
end

Ts = 0.01;
Max = 5/0.01;
t = [1:Max];
last_time = 0;
index = 1;
tic
while(1)
   time = uint32(toc*100); 
   if (time~=last_time)
       last_time = time;
       buffer = fgetl(s);
       buffer = strsplit(buffer);
       
       

       
   end    
end
