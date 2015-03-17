temporalBaseline= 180;
perpBaseline=150;
output = 1;
method = 'SBAS';
optimize = 1;

m = readcsv('metadata.csv');

disp('Add fields and read names for master and slave to structure m')
fldnm = 'Slavename';
m.(fldnm) = [];
fldnm = 'Mastername';
m.(fldnm) = [];
fileName = '../imagestack.txt';
inputfile = fopen(fileName);
while 1
    tline = fgetl(inputfile);
    if isempty(m.Mastername)==1
        m.Mastername = [m.Mastername {tline}];
    else
        m.Slavename = [m.Slavename {tline}];
    end
    if ~ischar(tline)
        break
    end
end
m.Slavename(end) = []
fclose(inputfile);

entries = length(m.Btemp);
sprintf('You have %i entries in metadata.csv',entries)




disp('optimize SBAS combinations')
    combinations = [m.Masterdate, m.Slavedate, m.Btemp, m.Bperp];
    combinations = combinations(1:end-1,1:4);
    M = combinations(1:end,1);
    rec = histc(M,unique(M));
    reccum = cumsum(rec);
    for i=1:length(rec)
%         if rec(i)<=5% max connections per master
%             %fprintf('%i \t %i \t %5.2f\n',reccum(i), combinations(j,3),combinations(j,4))
%         else
            for j=reccum(i)-rec(i)+1:reccum(i)
                fprintf('%i \t %i \t %i \t %5.2f\n',i, j, combinations(j,3),combinations(j,4))
                if abs(combinations(j,3))>temporalBaseline % temporal
                    combinations(j,1:4)=0;
                end
                if abs(combinations(j,4))>perpBaseline
                    combinations(j,1:4)=0;
                end
            end
%         end
    end
    
    combinations = unique(sortrows(combinations()),'rows');
    combinations = combinations(2:end,:);
    entries = length(combinations)
    
    disp('Wrinting new file combinations.csv')
    %cd ..
    if exist('combinations.csv')
    movefile('combinations.csv','combinations.csv.old');
    end
    if exist('SBAS')
    movefile('SBAS','SBAS_old');
    end
    dlmwrite('combinations.csv',combinations(:,1:2),'precision',10)
    
    dlmwrite('BtempBperp.txt',...
        ['Temporal Baseline:' sprintf('\t\t%i\n',temporalBaseline),...
        'Perpendicular Baseline:' sprintf('\t%i\n',perpBaseline),...
        'Number of combinations:' sprintf('\t%i',length(combinations))],...
        'delimiter','')
