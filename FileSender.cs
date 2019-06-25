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
        public static extern bool send_file(string address, string username, string filePath, byte[] requestID, bool directory);

        private SortedDictionary<string, string> destinationUsers;
        private string filePath;

        public FileSender(SortedDictionary<string, string> dUsers, string filePath_) {
            destinationUsers = dUsers;
            filePath = filePath_;
        }

        public void sendFile()
        {
            var isADirectory = isDirectory();
            if (isADirectory) {
                zipDirectory();
                filePath += ".zip";
            }
            Console.Out.WriteLine(filePath);

            foreach (var user in destinationUsers)  {
                byte[] requestIDBuff = new byte[31];
                string requestID;

                new Task(() => {
                    if (RequestProgress.Instance.getRunningUpload() < 5) {
                        if (send_file(user.Key, user.Value, filePath, requestIDBuff, isADirectory))
                        {
                            requestID = System.Text.Encoding.UTF8.GetString(requestIDBuff);
                            RequestProgress.Instance.addUploadRequest(requestID, user.Value, filePath);
                        }
                    } else
                    {
                        MessageBox.Show("Too many request. Wait for some request to complete!", "Too many request",
                            MessageBoxButtons.OK);
                    }
                }).Start();
            }
        }

        private void zipDirectory()
        {
            if (!File.Exists(filePath + ".zip"))
                ZipFile.CreateFromDirectory(filePath, filePath + ".zip");
        }

        private bool isDirectory() {

            // get the file attributes for file or directory
            var attr = File.GetAttributes(filePath);

            return attr.HasFlag(FileAttributes.Directory);
        }

    }
}
