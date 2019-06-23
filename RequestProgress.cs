﻿using System;
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
        private static ProgressForm uploadProgressForm = new ProgressForm("Uploading file");
        private static ProgressForm downloadProgressForm = new ProgressForm("Downloading file");

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ProgressBarDelegate(string requestId, int progress);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_progress_bar_callback(ProgressBarDelegate callback);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BeginDownloadDelegate(string requestId, string file_path);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_begin_download_callback(BeginDownloadDelegate callback);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void FileCompleteDelegate(string requestId, bool status, bool directory);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_file_sent_callback(FileCompleteDelegate callback);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_file_downloaded_callback(FileCompleteDelegate callback);

        private static ProgressBarDelegate progressBarDelegate;
        private static FileCompleteDelegate fileSentDelegate;
        private static FileCompleteDelegate fileDownloadedDelegate;
        private static BeginDownloadDelegate beginDownloadDelegate;
        private static SortedDictionary<string, DateTime> timeStamps = new SortedDictionary<string, DateTime>();
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
                        TimeSpan timespent = DateTime.Now - timeStamps[id];
                        int secondsremaining = (int)(timespent.TotalSeconds / progressBars[id].Value * (progressBars[id].Maximum - progressBars[id].Value));

                        var remainingTime = "";
                        if (secondsremaining > 60) {
                            remainingTime = ((int) (secondsremaining / 60)).ToString() + "m and " +
                                            (secondsremaining % 60).ToString() + "s remaining...";
                        } else {
                            remainingTime = secondsremaining.ToString() + "s remaining...";
                        }

                        progressBars[id].Value = progress; // Do all the ui thread updates here
                        progressBars[id].CustomText = progress.ToString() + "/100 % " + remainingTime;
                    }));

                }
            };

            fileSentDelegate = (id, status, directory) =>
            {
                parent.BeginInvoke(new Action(delegate() 
                {
                    if (status) {
                        progressBars[id].Value = 100;
                        TimeSpan timespent = DateTime.Now - timeStamps[id];
                        progressBars[id].CustomText = "Completed in " + timespent.Seconds;
                    }
                    else
                    {
                        if (progressBars[id].Value < 100)
                            progressBars[id].Value += 1;

                        progressBars[id].CustomText = progressBars[id].Value == 100 ? "The receiver may have encountered some problems." : "Failed";
                    }

                    uploadProgressForm.progressTerminated(id);
                    progressBars.Remove(id);
                }));

            };

            fileDownloadedDelegate = (id, status, directory) =>
            {
                if(directory)
                    Console.Out.Write("DIRECTORYYYYYYYYYYYYYYYYYYYYYYYYY");
                else
                    Console.Out.Write("NONNNNNNNNNNNNNNNNNNNNNNNNN DIRECTORYYYYYYYYYYYYYYYYYYYYYYYYY");

                parent.BeginInvoke(new Action(delegate ()
                {
                    if (status) {
                        TimeSpan timespent = DateTime.Now - timeStamps[id];
                        var remainingTime = "";
                        if (timespent.Seconds > 60) {
                            remainingTime = ((int) (timespent.Seconds / 60)).ToString() + "m and " +
                                            (timespent.Seconds % 60).ToString() + "s remaining...";
                        } else {
                            remainingTime = timespent.Seconds.ToString() + "s remaining...";
                        }
                        progressBars[id].CustomText = "Completed in " + remainingTime + "s";
                    }
                    else
                    {
                        if (progressBars[id].Value < 100)
                            progressBars[id].Value += 1;
                        progressBars[id].CustomText = "Failed";
                    }

                    downloadProgressForm.progressTerminated(id);
                    progressBars.Remove(id);
                }));

            };

            beginDownloadDelegate = (id, file_path) =>
            {
                parent.BeginInvoke(new Action(delegate ()
                {
                    addDownloadRequest(id, file_path);
                }));
            };

        }

        public void addDownloadRequest(string id, string file_path)
        {
            if(downloadProgressForm.IsDisposed)
                downloadProgressForm = new ProgressForm("Downloading file");
            if (!downloadProgressForm.Visible)
                downloadProgressForm.Show();
            downloadProgressForm.BringToFront();

            CustomProgressBar progressBar = new CustomProgressBar();
            progressBar.DisplayStyle = ProgressBarDisplayText.CustomText;
            progressBar.Value = 1;
            progressBar.CustomText = "Calculating remaining time";

            downloadProgressForm.addDownloadProgressbar(progressBar, id, file_path);
            progressBars.Add(id, progressBar);
            timeStamps.Add(id, DateTime.Now);
        }

        public void addUploadRequest(string id, string username, string file_path) {
            
            if(uploadProgressForm.IsDisposed)
                uploadProgressForm = new ProgressForm("Uploading file");

            if(!uploadProgressForm.Visible)
                uploadProgressForm.Show();
            uploadProgressForm.BringToFront();

            CustomProgressBar progressBar = new CustomProgressBar();
            progressBar.DisplayStyle = ProgressBarDisplayText.CustomText;
            progressBar.Value = 1;
            progressBar.CustomText = "Calculating remaining time";
            
            uploadProgressForm.addUploadProgressbar(progressBar, id, username, file_path);
            progressBars.Add(id, progressBar);
            timeStamps.Add(id, DateTime.Now);

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
            save_begin_download_callback(beginDownloadDelegate);
            save_file_downloaded_callback(fileDownloadedDelegate);
        }
    }
}
