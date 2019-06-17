using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace LanSharing
{
    class FileSender {

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool send_file(string address, string username, string filePath);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate bool AutoDownloadDelegate();
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_delegate_callback(AutoDownloadDelegate callback);

        private static AutoDownloadDelegate autoDownloadDelegate_;
        private SortedDictionary<string, string> destinationUsers;
        private string filePath;

        public FileSender(SortedDictionary<string, string> dUsers, string filePath_) {
            destinationUsers = dUsers;
            filePath = filePath_;

            autoDownloadDelegate_ = () =>
            {
                return true;
                //using (var fbd = new FolderBrowserDialog())
                //{

                //    DialogResult result = fbd.ShowDialog();

                //    if (result != DialogResult.OK || string.IsNullOrWhiteSpace(fbd.SelectedPath)) return "";

                //    return fbd.SelectedPath;
                //}
            };
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
