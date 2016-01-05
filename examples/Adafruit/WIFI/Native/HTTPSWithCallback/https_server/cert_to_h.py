import sys

if (len(sys.argv) < 4):
  print "Wrong syntax!"
  print "Usage: python certs_to_h.py <cert_file> <output_variable_name> <output_file_name>"
  sys.exit()

f_in = open(sys.argv[1], 'r')
f_out = open("../"+sys.argv[3]+".h", 'w')

f_out.write(
"#ifndef _CERTIFICATE_H_\r\n\
#define _CERTIFICATE_H_\r\n\
\r\n\
#ifdef __cplusplus\r\n\
extern \"C\"\r\n\
{\r\n\
#endif\r\n\
\r\n")

f_out.write("const char ")
f_out.write(sys.argv[2])
f_out.write("[] =")

for line in f_in:
    f_out.write("\r\n\"")
    line = line.strip()
    f_out.write(line)
    f_out.write("\\r\\n\"")

f_out.write(";\r\n")
f_out.write(
"\r\n\
#ifdef __cplusplus\r\n\
} /* extern \"C\" */\r\n\
#endif\r\n\
\r\n\
#endif /* ifndef _CERTIFICATE_H_ */")

f_in.close()
f_out.close()