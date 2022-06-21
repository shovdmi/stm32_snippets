OUT_FILE = "build/out.txt"
File.delete OUT_FILE if File.exists? OUT_FILE

pipe = IO.popen("arm-none-eabi-gdb"   \
                " -x gdb_script -ex=continue" \
                " --batch-silent --quiet " +  ARGV[0] + \
                " 2>  #{OUT_FILE}  ")
#pipe = IO.popen(["arm-none-eabi-gdb",   
#                " -x gdb_script -ex=continue" \
#                " --batch-silent --quiet " +  ARGV[0] + \
#                " 2>  #{OUT_FILE} ", :err=>[:child, :out]])

trap("INT") { Process.kill("KILL", pipe.pid); exit }

Process.wait(pipe.pid)
if File.exists? OUT_FILE
    file_contents = File.read OUT_FILE
    print file_contents
else
    print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" \
          "! Program was not simulated ? !\n" \
          "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
end
