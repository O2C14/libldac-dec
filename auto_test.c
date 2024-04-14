#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir("../")) != NULL)
  {
    // 打开当前目录
    while ((ent = readdir(dir)) != NULL)
    {
      // 读取目录内容
      char *filename = ent->d_name;
      int len = strlen(filename);
      if (len >= 7)
      {
        // 过滤后缀名
        char *suffix = filename + len - 7;
        if (strcmp(suffix, "_HQ.wav") == 0 ||
            strcmp(suffix, "_SQ.wav") == 0 ||
            strcmp(suffix, "_MQ.wav") == 0)
        {
          continue;
        }
      }
      if (len > 4)
      {
        // 过滤后缀名
        char *suffix = filename + len - 4;
        if (strcmp(suffix, ".wav") == 0)
        {
          // 匹配wav后缀
          memset(suffix, 0, 4);
          printf("%s\n", filename);
          char current_path[128];
          memset(current_path, 0, sizeof(current_path));
          getcwd(current_path, sizeof(current_path));
          char ldac_encoder[128];
          memset(ldac_encoder, 0, sizeof(ldac_encoder));
          strcat(ldac_encoder, current_path);
          strcat(ldac_encoder, "\\ldac_encoder.exe ");
          strcat(ldac_encoder, filename);
          puts(ldac_encoder);
          system(ldac_encoder);
          system(ldac_encoder);
          system(ldac_encoder);
          char ldac_decoder[128];
          memset(ldac_decoder, 0, sizeof(ldac_decoder));
          strcat(ldac_decoder, current_path);
          strcat(ldac_decoder, "\\ldac_decoder.exe ");
          strcat(ldac_decoder, filename);
          puts(ldac_decoder);
          system(ldac_decoder);
          system(ldac_decoder);
          system(ldac_decoder);
        }
      }
    }
    closedir(dir);
  }
  else
  {
    perror("");
  }

  return 0;
}