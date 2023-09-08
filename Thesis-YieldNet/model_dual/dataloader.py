from tensorflow import keras
from osgeo import gdal
import numpy as np
import json
import os
import csv


class DataLoader(keras.utils.Sequence):
    """
    Custom data loader for training and validation, which inherits from keras Sequence.

    Note: Every Sequence must implement the __getitem__ and the __len__ methods. If you want to modify your dataset between epochs you may implement on_epoch_end. The method __getitem__ should return a complete batch.
    """

    def __init__(self, dataloader_config_fpath):
        """
        Params:
        dataloader_config_fpath (str): Path to the data loader configuration
            settings JSON file, defining YieldNet's input-output data configuration.

        seed (int): Random seed used for shuffling the training samples before
            each epoch.
        """

        # Read config file
        with open(dataloader_config_fpath, "r") as f:
            self.config = json.load(f)
        self.train_sample_IDs = self.create_sample_IDs(dataset="train")
        self.val_sample_IDs = self.create_sample_IDs(dataset="val")
        self.test_sample_IDs = self.create_sample_IDs(dataset="test")
        with open(os.path.join(self.config["raw_folder"], "city_yield.csv"), "r") as f:
            reader = csv.reader(f)
            self.city_yield = list(reader)[1:]  # skip header
        
    def __getitem__(self):
        pass

    def __len__(self):
        pass

    def data_generation(self, year, ct_adcode):
        """
        Generate input-output data for YieldNet for a given year and ct_adcode.

        Parameters:
        year (int): year of the sample to generate, with format YYYY.
        ct_adcode (str): ct_adcode of the county.

        Returns:
        tuple: (X1, X2, y) where X1 is ndarray of LST with shape (bins, times, bands), X2 is ndarray of Gpp+LAI+ET with shape (bins, times, bands) and y is truth yield value or None.
        """

        ########################################################################
        # OUTPUT LABELS
        ########################################################################

        # Get y by searching for the corresponding yield value in the yield table
        y = next(
            (
                y[3]
                for y in self.city_yield
                if y[0] == year and y[2] == ct_adcode
            ),
            None,
        )
        # If y is None, then the county does not have a yield value for the given year
        if y == 0 or y is None:
            return None, None, None
        y = np.float32(y)

        ########################################################################
        # INPUT FEATURES
        ########################################################################

        # Batch tensor
        tiffs = self.config["modis"]
        hist_list = self.config["hist_list"]
        bins = self.config["hist_bins"]
        band_index = 0
        X1 = []  # LST
        X2 = []  # Gpp+LAI+ET
        for tiff in tiffs:
            bands = tiff["bands"]
            calc_hist = self.calc_hist(
                tiff["name"],
                bands,
                hist_list[band_index : band_index + len(bands)],
                bins,
                year,
                ct_adcode,
            )
            if tiff["name"] == "MOD09A1":
                X1.append(calc_hist)
            else:
                X2.append(calc_hist)
            band_index += len(bands)
        # concatecate all the bands
        X1 = np.concatenate(X1, axis=2)
        X2 = np.concatenate(X2, axis=2)
        return X1, X2, y

    def calc_hist(self, modis, bands, hists, bins, year, ct_adcode):
        """
        Load tiff file and calculate histogram.

        Parameters:
        modis: modis dataset name.
        bands (list): list of band name(str).
        hist (list): list of  min and max value of the histogram, with format [[min, max],...].
        bins (int): number of bins.
        year (int): year of the sample to generate, with format YYYY.
        ct_adcode (str): ct_adcode of the county.

        Return:
        hist (np.array): histogram with shape (bins, times, bands) or None if the image is invalid.
        """

        modis_path = os.path.join(self.config["raw_folder"], "masked", modis)
        tiff_list = [
            os.path.join(
                modis_path,
                date,
                next(
                    f
                    for f in os.listdir(os.path.join(modis_path, date))
                    if ct_adcode in f
                ),
            )
            for date in os.listdir(modis_path)
            if str(year) in date and os.path.isdir(os.path.join(modis_path, date))
        ]
        tiff_list = self.sort_list(tiff_list)
        hist = np.zeros((bins, len(tiff_list), len(bands)))
        for time_index, file in enumerate(tiff_list):
            ds = gdal.Open(file, gdal.GA_ReadOnly)
            bands_num = ds.RasterCount
            for i in range(bands_num):
                band = ds.GetRasterBand(i + 1)
                band_name = band.GetDescription()
                if band_name in bands:
                    band_array = band.ReadAsArray()  # rows x cols
                    band_index = bands.index(band_name)
                    hist_range = hists[band_index]
                    bin_seq = np.linspace(hist_range[0], hist_range[1], bins + 1)
                    density, _ = np.histogram(band_array, bin_seq, density=False)
                    density_sum = density.sum()
                    if density_sum == 0:
                        hist[:, time_index, band_index] = density
                    else:
                        hist[:, time_index, band_index] = density / float(density_sum)
            ds = None
        return hist

    def create_sample_IDs(self, dataset="train"):
        """
        Create a list of sample dicts containing the year and ct_adcode, using the the train, val, or test sets based on the configuration JSON file start & end points for each dataset.
        """
        years = [
            year
            for year in range(
                self.config["sample_IDs"][f"obs_{dataset}_years"]["start"],
                self.config["sample_IDs"][f"obs_{dataset}_years"]["end"] + 1,
            )
            if not str(year)
            in self.config["sample_IDs"][f"obs_{dataset}_years"]["except"]
        ]
        with open(os.path.join(self.config["raw_folder"], "city_yield.csv"), "r") as f:
            reader = csv.reader(f)
            yield_arr = list(reader)[1:]  # skip header
        return [
            {"year": y[0], "ct_adcode": y[2]}
            for y in yield_arr
            if int(y[0]) in years
        ]

    def sort_list(self, flist):
        """
        Sort the chronological list by time.

        Parameters:
        flist(list): list of tiff file paths.

        Return:
        flist(list): sorted list.
        """
        flist.sort()
        return flist

    def check_file(self, ds):
        """
        Check if the data source is valid.

        Parameters:
        ds: data source.

        Return:
        bool: True if the file is valid, else False.
        """
        band = ds.GetRasterBand(1)
        data = band.ReadAsArray().flatten()
        no_data_value = band.GetNoDataValue()
        data = [x for x in data if x != no_data_value]
        if len(data) > 20:
            return True
        else:
            return False
