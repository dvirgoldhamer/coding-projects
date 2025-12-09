import fs from 'fs';
import csv from 'csv-parser';

export default function readCSV<T>(filePath: string): Promise<T[]> 
{
    return new Promise((resolve, reject) => 
    {
        const results: T[] = [];
        fs.createReadStream(filePath)
        .pipe(csv())
        .on('data', row => results.push(row as T)) //end of row
        .on('end', () => resolve(results)) //end of file
        .on('error', reject);
    });
}
