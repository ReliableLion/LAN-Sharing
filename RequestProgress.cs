using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LanSharing
{
    class RequestProgress {

        private static readonly RequestProgress instance = new RequestProgress();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ProgressBarDelegate(string requestId, int progress);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_progress_callback(ProgressBarDelegate callback);

        private static ProgressBarDelegate progressBarDelegate;
        private static SortedDictionary<string, ProgressBar> progressBars;

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static RequestProgress() {
        }

        private RequestProgress()
        {
            progressBarDelegate = (id, progress) => {
                if (progressBars.ContainsKey(id)) {
                    progressBars[id].Value = progress;
                }
            };
        }

        public void addRequest(string id, string username, string file_path) {
            ProgressBar progressBar = new ProgressBar();
            progressBar.Text = "Sending " + Path.GetFileName(file_path) + " to " + username;
            progressBar.Show();
            progressBars.Add(id, progressBar);
        }

        public void initialize(){saveCallback();}

        public static RequestProgress Instance {
            get
            {
                return instance;
            }
        }

        private void saveCallback() {
            save_progress_callback(progressBarDelegate);
        }

    }
}
