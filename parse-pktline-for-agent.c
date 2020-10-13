#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *test_with_nul_byte = "00adwant 49cde2bb7e8fe922d8d29f6a0b25c42fe2e1424c\0multi_ack_detailed\0side-band-64k\0thin-pack\0no-progress\0ofs-delta\0deepen-since\0deepen-not\0agent=git/2.27.0.2890.g910e898871";
char *test_with_spaces = "00adwant 49cde2bb7e8fe922d8d29f6a0b25c42fe2e1424c multi_ack_detailed side-band-64k thin-pack no-progress ofs-delta deepen-since deepen-not agent=git/2.27.0.2890.g910e898871";
char *test_gpv2 = "0025agent=git/2.27.0.2890.g910e898871";

void parse(char *p, int gpv) {
  char result[200];

  if ((gpv == 2) && (strncmp(p+4, "agent=", 6) == 0)) {
    int len = strlen(p);
    strlcpy(result, p+10, len-9);
  } else {
    while (*p) {
      char *agent_key_pointer = strstr(p, "agent=");
      if (agent_key_pointer != NULL) {
        // Set agent_key_value_length to length of rest of string
        int agent_key_value_length = strlen(agent_key_pointer);
        char *agent_end = strstr(agent_key_pointer, " ");
        if (agent_end == NULL) {
          // Update agent_key_value_length if it ends with newline
          agent_key_value_length = (agent_key_pointer[agent_key_value_length - 1] == '\n') ? agent_key_value_length : agent_key_value_length + 1;
        } else {
          agent_key_value_length = agent_end - agent_key_pointer;
        }
        // Save agent
        int key_len = 6; // "agent="
        strlcpy(result, agent_key_pointer + key_len, agent_key_value_length - key_len);
        break;
      }
      p += strlen(p) + 1;
    }
  }

  printf("%s\n", result);
  return;
}

int main() {
  parse(test_with_nul_byte, 1);
  parse(test_with_spaces, 1);
  parse(test_gpv2, 2);
  return 0;
}
