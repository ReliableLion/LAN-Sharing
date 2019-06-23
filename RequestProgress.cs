using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Windows.Threading;
using System.Windows.Shell;

namespace LanSharing
{
    class RequestProgress {

        private static readonly RequestProgress instance = new RequestProgress();
        private static ProgressForm progressForm = new ProgressForm();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ProgressBarDelegate(string requestId, int progress);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_progress_bar_callback(ProgressBarDelegate callback);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void FileSentDelegate(string requestId, bool status);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_file_sent_callback(FileSentDelegate callback);

        private static ProgressBarDelegate progressBarDelegate;
        private static FileSentDelegate fileSentDelegate;
        private static SortedDictionary<string, CustomProgressBar> progressBars = new SortedDictionary<string, CustomProgressBar>();
        private MainForm parent;

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static RequestProgress() {
        }

        private RequestProgress()
        {
            progressBarDelegate = (id, progress) => {
                Console.Out.WriteLine("ID: " + id + " PROGRESS: " + progress);
                if (progressBars.ContainsKey(id)) {
                    //progressBars[id].Value = progress;
                    parent.BeginInvoke(new Action(delegate() 
                    {       
                        progressBars[id].Value = progress; // Do all the ui thread updates here
                        progressBars[id].CustomText = progress.ToString() + "/100";
                    }));

                }
            };

            fileSentDelegate = (id, status) =>
            {

                parent.BeginInvoke(new Action(delegate() 
                {       
                    if(status)
                        progressBars[id].CustomText = "Complete";
                    else
                    {
                        if(progressBars[id].Value < 100)
                            progressBars[id].Value += 1;
                        progressBars[id].CustomText = "Failed";
                    }

                    progressForm.progressTerminated(id);
                    progressBars.Remove(id);
                }));

            };

        }

        public void addRequest(string id, string username, string file_path) {
            if(!progressForm.Visible)
                progressForm.Show();
            progressForm.BringToFront();

            CustomProgressBar progressBar = new CustomProgressBar();
            progressBar.DisplayStyle = ProgressBarDisplayText.CustomText;
            progressBar.CustomText = "";
            //progressBar.Text = "Sending " + Path.GetFileName(file_path) + " to " + username;
            
            progressForm.addProgressbar(progressBar, id, username, file_path);
            progressBars.Add(id, progressBar);

            //backgroundWorker.RunWorkerAsync();
        }

        public void initialize(MainForm mainForm)
        {
            parent = mainForm;
            saveCallback();
        }

        public static RequestProgress Instance {
            get
            {
                return instance;
            }
        }

        private void saveCallback() {
            save_progress_bar_callback(progressBarDelegate);
            save_file_sent_callback(fileSentDelegate);
        }
    }
}
