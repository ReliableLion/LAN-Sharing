using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using System.Runtime.InteropServices;

namespace LanSharing
{
    class FileSender {

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool send_file(string address, string username, string filePath);

        private SortedDictionary<string, string> destinationUsers;
        private string filePath;

        public FileSender(SortedDictionary<string, string> dUsers, string filePath_) {
            destinationUsers = dUsers;
            filePath = filePath_;
        }

        public void sendFile() {
            if (isDirectory()) {
                zipDirectory();
                filePath += ".zip";
            }
            Console.Out.WriteLine(filePath);

            foreach (var user in destinationUsers)  {
                send_file(user.Key, user.Value, filePath);
            }
        }

        private void zipDirectory() {
            ZipFile.CreateFromDirectory(filePath, filePath + ".zip");
        }

        private bool isDirectory() {

            // get the file attributes for file or directory
            var attr = File.GetAttributes(filePath);

            return attr.HasFlag(FileAttributes.Directory);
        }

    }
}
