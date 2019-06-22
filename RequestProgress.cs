using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Windows.Threading;
using System.Windows.Shell;
using System.Windows;

namespace LanSharing
{
    class RequestProgress {

        private static readonly RequestProgress instance = new RequestProgress();
        private static ProgressForm progressForm = new ProgressForm();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ProgressBarDelegate(string requestId, int progress);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_progress_bar_callback(ProgressBarDelegate callback);

        private static ProgressBarDelegate progressBarDelegate;
        private static SortedDictionary<string, ProgressBar> progressBars = new SortedDictionary<string, ProgressBar>();
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
                    System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate() 
                    {       
                        progressBars[id].Value = progress; // Do all the ui thread updates here
                    }));

                }
            };
        }

        public void addRequest(string id, string username, string file_path) {
            if(!progressForm.Visible)
                progressForm.Show();
            progressForm.BringToFront();

            ProgressBar progressBar = new ProgressBar();
            //progressBar.Text = "Sending " + Path.GetFileName(file_path) + " to " + username;
            
            progressForm.addProgressbar(progressBar);
            progressBars.Add(id, progressBar);

            //backgroundWorker.RunWorkerAsync();
        }

        public void initialize()
        {
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
        }
    }
}
