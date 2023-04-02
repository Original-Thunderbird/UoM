/**
 * Basic Inverted Index
 * 
 * This Map Reduce program should build an Inverted Index from a set of files.
 * Each token (the key) in a given file should reference the file it was found 
 * in. 
 * 
 * The output of the program should look like this:
 * sometoken [file001, file002, ... ]
 * 
 * @author Kristian Epps
 */
package uk.ac.man.cs.comp38211.exercise;

import java.io.*;
import java.util.*;
import java.util.regex.Pattern;
import java.lang.Math;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.log4j.Logger;

import uk.ac.man.cs.comp38211.io.array.ArrayListWritable;
import uk.ac.man.cs.comp38211.io.map.HashMapWritable;
import uk.ac.man.cs.comp38211.ir.Stemmer;
import uk.ac.man.cs.comp38211.ir.StopAnalyser;
import uk.ac.man.cs.comp38211.util.XParser;


public class BasicInvertedIndex extends Configured implements Tool {
    private static final Logger LOG = Logger.getLogger(BasicInvertedIndex.class);
    
    public static class Map extends Mapper<Object, Text, Text, ArrayListWritable<Writable>> {

        // INPUTFILE holds the name of the current file
        private final static Text INPUTFILE = new Text();
        // TOKEN should be set to the current token rather than creating a 
        // new Text object for each one
        @SuppressWarnings("unused")
        private final static Text TOKEN = new Text();
        // The StopAnalyser class helps remove stop words
        @SuppressWarnings("unused")
        private StopAnalyser stopAnalyser = new StopAnalyser();
        private int rownum = 0; 
        
        // The stem method wraps the functionality of the Stemmer
        // class, which trims extra characters from English words
        // Please refer to the Stemmer class for more comments
        @SuppressWarnings("unused")
        private String stem(String word) {
            Stemmer s = new Stemmer();
            // A char[] word is added to the stemmer with its length,
            // then stemmed
            s.add(word.toCharArray(), word.length());
            s.stem();
            // return the stemmed char[] word as a string
            return s.toString();
        }
        
        // This method gets the name of the file the current Mapper is working
        // on
        @Override
        public void setup(Context context) {
            String inputFilePath = ((FileSplit) context.getInputSplit()).getPath().toString();
            String[] pathComponents = inputFilePath.split("/");
            INPUTFILE.set(pathComponents[pathComponents.length - 1]);
        }
         
        // TODO
        // This Mapper should read in a line, convert it to a set of tokens
        // and output each token with the name of the file it was found in
        HashMap<String, ArrayListWritable<Writable>> cache = new HashMap<>();
        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
        	rownum++;
        	int colnum = 0;
        	// The line is broken up and turned into an iterator 
            String line = value.toString();
            line = line.toLowerCase();
            //remove all reference labels like [1] 
            line = line.replaceAll("\\[[\\d]+\\]", "");
            StringTokenizer itr = new StringTokenizer(line);
            PrintStream ps = new PrintStream("output/log.txt");
            System.setOut(ps);
            
            // While there are more tokens in the input, output with value 1
            while (itr.hasMoreTokens()) {
            	colnum++;

            	String raw = itr.nextToken();
            	//preserve format of web
            	if(raw.contains("http")) {
            		raw = raw.replaceAll("\"", "");
            	}
            	else if(raw.contains("isbn"));//preserve format of ISBN
            	else {
            		raw = raw.replaceAll("[^a-zA-Z0-9- ]", "");
            	}
            	if (raw.equals("")) {
            		continue;
            	}
            	//remove single chars and some meanlinglsess tokens that have regular pattern
            	if(Pattern.matches("^\\d+$|^.$|(^[0-9][a-z][0-9][0-9]$)", raw)) {
            		continue;
            	}
            	//stopword removal
            	if(stopAnalyser.isStopWord(raw)) {
            		continue;
            	}
            	//stemmer
            	String finkey = stem(raw);
            	
            	Text index = new Text("<"+rownum+","+colnum+">");
            	//save filename and indices all in a arraylist
            	//new token, initialise array of index, insert
            	if(cache.containsKey(finkey)==false) {
            		ArrayListWritable<Writable> indexlist = new ArrayListWritable<>();
            		indexlist.add(INPUTFILE);
            		indexlist.add(index);
            		cache.put(finkey,indexlist);
            	}
            	else {//old token, add 1 to frequency, replace
            		ArrayListWritable<Writable> oldarr = new ArrayListWritable<>(cache.get(finkey));
            		ArrayListWritable<Writable> newarr = new ArrayListWritable<>(cache.get(finkey));
            		newarr.add(index);
            		cache.replace(finkey, oldarr, newarr);
            	}
            	
        		//in-mapper aggregation not used
        		//{
//            	ArrayListWritable<Writable> indexlist = new ArrayListWritable<>();
//        		indexlist.add(INPUTFILE);
//        		indexlist.add(index);
//        		TOKEN.set(finkey);
//        		context.write(TOKEN, indexlist);
        		//}
            }
        }
        
        protected void cleanup(Context context) throws IOException, InterruptedException {
            //transfer entries in Hashmap to context
            for(HashMap.Entry<String, ArrayListWritable<Writable>> entry: cache.entrySet()) {
            	TOKEN.set(entry.getKey());

            	context.write(TOKEN, entry.getValue());
            }
        }
    }

    public static class Reduce extends Reducer<Text, ArrayListWritable<Writable>, Text, HashMapWritable<Text,ArrayListWritable<Writable>>> {

    	ArrayListWritable<Writable> list = new ArrayListWritable<>();
        // TODO
        // This Reduce Job should take in a key and an iterable of file names
        // It should convert this iterable to a writable array list and output
        // it along with the key
        public void reduce(Text key, Iterable<ArrayListWritable<Writable>> values, Context context) throws IOException, InterruptedException {
        	System.out.print('\n');
            Iterator<ArrayListWritable<Writable>> iter = values.iterator();
            //statistic for frequency in each document
            HashMapWritable<Text,ArrayListWritable<Writable>> cache = new HashMapWritable<>();

            //list.clear();
            System.out.println(key);
            while (iter.hasNext()) {
            	ArrayListWritable<Writable> tmp = new ArrayListWritable<>(iter.next());
            	Text doc = new Text(tmp.get(0).toString());
            	tmp.remove(0);
            	//put in local cache
            	//new token, insert
            	if(cache.containsKey(doc)==false) {
            		cache.put(doc, tmp);
            	}
            	//old token, replace
            	else {
            		ArrayListWritable<Writable> oldarr = new ArrayListWritable<>(cache.get(doc));
            		ArrayListWritable<Writable> newarr = new ArrayListWritable<>(cache.get(doc));
            		newarr.addAll(tmp);
            		cache.replace(doc, oldarr, newarr);
            	}
            }
            
            //tf-idf calculation
            int df = cache.size();
            for(HashMap.Entry<Text, ArrayListWritable<Writable>> entry: cache.entrySet()) {
            	ArrayListWritable<Writable> oldarr = new ArrayListWritable<>(entry.getValue());
            	ArrayListWritable<Writable> newarr = new ArrayListWritable<>(entry.getValue());
            	int tf = oldarr.size();
            	//the formula for calculating tf-idf
            	DoubleWritable tfidf = new DoubleWritable((1+Math.log((double)tf))*Math.log10(6.0/(double)df));
        		newarr.add(tfidf);
        		//print more foramtted result in my own file
        		System.out.print(entry.getKey());
        		System.out.print(' ');
        		System.out.print(newarr);
        		System.out.print('\n');
        		cache.replace(entry.getKey(), oldarr, newarr);
            }
            // Output the key with the token
            context.write(key, cache);
        }
    }

    // Lets create an object! :)
    public BasicInvertedIndex() {
    	System.setOut(System.out);
    	System.out.print("test");
    }

    // Variables to hold cmd line args
    private static final String INPUT = "input";
    private static final String OUTPUT = "output";
    private static final String NUM_REDUCERS = "numReducers";

    @SuppressWarnings({ "static-access" })
    public int run(String[] args) throws Exception {
        // Handle command line args
        Options options = new Options();
        options.addOption(OptionBuilder.withArgName("path").hasArg()
                .withDescription("input path").create(INPUT));
        options.addOption(OptionBuilder.withArgName("path").hasArg()
                .withDescription("output path").create(OUTPUT));
        options.addOption(OptionBuilder.withArgName("num").hasArg()
                .withDescription("number of reducers").create(NUM_REDUCERS));

        CommandLine cmdline = null;
        CommandLineParser parser = new XParser(true);

        try {
            cmdline = parser.parse(options, args);
        }
        catch (ParseException exp) {
            System.err.println("Error parsing command line: "
                    + exp.getMessage());
            System.err.println(cmdline);
            return -1;
        }
        // If we are missing the input or output flag, let the user know
        if (!cmdline.hasOption(INPUT) || !cmdline.hasOption(OUTPUT)) {
            System.out.println("args: " + Arrays.toString(args));
            HelpFormatter formatter = new HelpFormatter();
            formatter.setWidth(120);
            formatter.printHelp(this.getClass().getName(), options);
            ToolRunner.printGenericCommandUsage(System.out);
            return -1;
        }

        // Create a new Map Reduce Job
        Configuration conf = new Configuration();
        Job job = new Job(conf);
        String inputPath = cmdline.getOptionValue(INPUT);
        String outputPath = cmdline.getOptionValue(OUTPUT);
        int reduceTasks = cmdline.hasOption(NUM_REDUCERS) ? Integer.parseInt(cmdline.getOptionValue(NUM_REDUCERS)) : 1;

        // Set the name of the Job and the class it is in
        job.setJobName("Basic Inverted Index");
        job.setJarByClass(BasicInvertedIndex.class);
        job.setNumReduceTasks(reduceTasks);
        
        // Set the Mapper and Reducer class (no need for combiner here)
        job.setMapperClass(Map.class);
        job.setReducerClass(Reduce.class);
        
        // Set the Output Classes
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(ArrayListWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(HashMapWritable.class);

        // Set the input and output file paths
        FileInputFormat.setInputPaths(job, new Path(inputPath));
        FileOutputFormat.setOutputPath(job, new Path(outputPath));
        
        // Time the job whilst it is running
        long startTime = System.currentTimeMillis();
        job.waitForCompletion(true);
        LOG.info("Job Finished in " + (System.currentTimeMillis() - startTime)
                / 1000.0 + " seconds");

        // Returning 0 lets everyone know the job was successful
        return 0;
    }

    public static void main(String[] args) throws Exception {
        ToolRunner.run(new BasicInvertedIndex(), args);
    }
}
