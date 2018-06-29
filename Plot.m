function []= Plot()

s = serial('COM3','BAUD',115200);
try
    fopen(s);
catch err
    fclose(instrfind);
   
    error('NOT CONNECTED.');
end


Ts = 0.04;
Max = (10/Ts);
t = (1:Max);
last_time = 0;
index = 0;
figure
h1 = animatedline;
h1.Color = 'blue';
h2=animatedline;
h2.Color = 'red';
h3=animatedline;
h3.Color = 'green';
ylim([-150,150]);
tic
while(1)
   time = uint32(toc/Ts); 
   if(time>500)
    disp('END OF THE PROGRAM');
    
    return
   end
   if (time~=last_time)
       
     
       last_time = time;
       buffer = fgetl(s);
       buffer = strsplit(buffer);
         if(length(buffer)==3)
         addpoints(h1,index,str2double(buffer(1)));
         addpoints(h2,index,str2double(buffer(2)));
         addpoints(h3,index,str2double(buffer(3))/48);
         end
       xlim([index-2*Max index]);
       index = index + 1;

   end
   drawnow
   hold on 
end
