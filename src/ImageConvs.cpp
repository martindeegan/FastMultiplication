#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <random>
#include <string>
#include <valarray>
#include <vector>

#include <opencv2/opencv.hpp>

constexpr int t = 1;
static int m = 0;

using Complex = std::complex<double>;

void FFT(std::valarray<Complex> &x) {
  const size_t N = x.size();
  if (N <= 1)
    return;

  std::valarray<Complex> even = x[std::slice(0, N / 2, 2)];
  std::valarray<Complex> odd = x[std::slice(1, N / 2, 2)];

  FFT(even);
  FFT(odd);

  for (size_t k = 0; k < N / 2; ++k) {
    Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
    x[k] = even[k] + t;
    x[k + N / 2] = even[k] - t;
  }
}

void invFFT(std::valarray<Complex> &x) {
  x = x.apply(std::conj);
  FFT(x);
  x = x.apply(std::conj);
  x /= x.size();
}

void multiply(std::valarray<Complex> &i, std::valarray<Complex> &j) {
  std::cout << "Multiplying a " << i.size() << " integer with a " << j.size()
            << " integer." << std::endl;

  FFT(i);
  FFT(j);

  for (size_t k = 0; k < i.size(); k++) {
    i[k] *= j[k];
  }

  invFFT(i);
}

size_t upper_power_of_two(size_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}

void pad_mat(cv::Mat &m, size_t n) {
  cv::Mat padded(cv::Size(n, n), CV_64FC1, cv::Scalar(0));
  cv::Mat area = padded.rowRange(0, m.rows).colRange(0, m.cols);
  m.copyTo(padded(cv::Rect(0, 0, m.cols, m.rows)));
  cv::normalize(padded, padded, 0, 1, cv::NORM_MINMAX);
  m = padded;
}

std::valarray<Complex> get_mat_int(cv::Mat &m) {
  double *b = reinterpret_cast<double *>(m.data);
  double *e = b + (m.rows * m.cols);
  std::vector<Complex> i_vec(b, e);
  std::valarray<Complex> i(i_vec.data(), i_vec.size());
  std::vector<double> v;
  for (auto c : i_vec) {
    v.push_back(c.real());
  }
  return i;
}

cv::Mat get_mat(std::valarray<Complex> &a, size_t n) {
  std::vector<double> v;
  for (Complex c : a) {
    v.push_back(c.real());
  }
  cv::Mat m(n, n, CV_64FC1);
  memcpy(m.data, v.data(), v.size() * sizeof(double));
  cv::normalize(m, m, 0, 1, cv::NORM_MINMAX);
  return m;
}

int main(int argc, char **argv) {
  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  cv::imshow("Unedited image", image);

  cv::Mat identity_kernel(cv::Size(3, 3), CV_64FC1, cv::Scalar(0));
  identity_kernel.at<double>(1, 1) = 1;
  cv::Mat edge_kernel(cv::Size(3, 3), CV_64FC1, cv::Scalar(0));
  edge_kernel.at<double>(0, 0) = edge_kernel.at<double>(0, 1) =
      edge_kernel.at<double>(0, 2) = edge_kernel.at<double>(1, 0) =
          edge_kernel.at<double>(1, 2) = edge_kernel.at<double>(2, 0) =
              edge_kernel.at<double>(2, 1) = edge_kernel.at<double>(2, 2) = -1;
  edge_kernel.at<double>(1, 1) = 8;
  cv::Mat blur_kernel(cv::Size(10, 10), CV_64FC1, cv::Scalar(1));

  size_t n = upper_power_of_two(std::max(image.cols, image.rows));

  pad_mat(image, n);
  pad_mat(edge_kernel, n);
  pad_mat(blur_kernel, n);
  pad_mat(identity_kernel, n);

  cv::Mat blur_copy = image;

  imshow("Padded Image", image);

  std::valarray<Complex> img_int = get_mat_int(image);
  std::valarray<Complex> ker_int = get_mat_int(edge_kernel);
  multiply(img_int, ker_int);
  cv::Mat edge_det_image = get_mat(img_int, n);

  imshow("Edge Detection",
         edge_det_image.colRange(0, image.cols).rowRange(0, image.rows));

  std::valarray<Complex> img_int2 = get_mat_int(blur_copy);
  std::valarray<Complex> ker_int2 = get_mat_int(blur_kernel);
  multiply(img_int2, ker_int2);
  cv::Mat blurred_image = get_mat(img_int2, n);

  imshow("Blurred Image",
         blurred_image.colRange(0, image.cols).rowRange(0, image.rows));

  cv::resize(identity_kernel, identity_kernel, cv::Size(500, 500), 0, 0,
             CV_INTER_AREA);
  cv::resize(edge_kernel, edge_kernel, cv::Size(500, 500), 0, 0, CV_INTER_AREA);
  cv::resize(blur_kernel, blur_kernel, cv::Size(500, 500), 0, 0, CV_INTER_AREA);

  //   imshow("Identity Kernel", identity_kernel);
  //   imshow("Edge Detection Kernel", edge_kernel);
  //   imshow("Blur Kernel", blur_kernel);

  cv::waitKey(0);
}