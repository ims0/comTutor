/*********************************************************
  > File Name: app.c
  > Author: ims
  > Created Time: Sat 15 May 2021 10:15:54 PM CST
 *********************************************************/

#include "myioctl.h"
#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
int main(void) {
  int ret = 0;
  string file_path = "/dev/";
  file_path += misc_dev_name;
  int dev_fd = open(file_path.c_str(), O_RDWR);
  if (dev_fd < 0) {
    cerr << "open :" << file_path << " fail," << strerror(errno) << endl;
    return -1;
  }
  cout << "open device fd:" << dev_fd << endl;

  struct miscdata data;
  data.val = 100;
  data.str = "misc platform device";
  data.size = sizeof("misc platform device");
  auto start = std::chrono::steady_clock::now();
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_SET, &data)) < 0) {
    cout << "IOC_SET failed" << endl;
  }
  auto end = std::chrono::steady_clock::now();
  auto elapsed_time = end - start;
  std::chrono::duration<double> diff = end - start;
  cout << "duration:" << elapsed_time.count() << "ns" <<endl;
  cout << "duration:" << diff.count() << "s" <<endl;
  // io get
  struct miscdata getdata;
  start = std::chrono::steady_clock::now();
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_GET, &getdata)) < 0) {
    printf("ioctl get fail:%s\n", strerror(errno));
  }
  end = std::chrono::steady_clock::now();

  elapsed_time = end - start;
  diff = end - start;
  cout << "duration:" << elapsed_time.count() << "ns" << endl;
  cout << "duration:" << diff.count() << "s" <<endl;
  cout << "ioctl get data:val:" << getdata.val << "str:" << getdata.str << endl;

  // io printk
  if ((ret = ioctl(dev_fd, FELLOW_MISC_IOC_PRINT, NULL)) < 0) {
    cerr << "ioctl printk fail: " << strerror(errno) << endl;
  }
  close(dev_fd);
  return ret;
}
