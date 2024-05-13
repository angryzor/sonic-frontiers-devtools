#pragma once

template<typename T, int AmbientDim>
struct Ray {
    Eigen::Matrix<T, AmbientDim, 1> start;
    Eigen::Matrix<T, AmbientDim, 1> end;

    Ray() : start{}, end{} {}
    Ray(const Ray& other) : start{ other.start }, end{ other.end } {}
    Ray(const Eigen::Matrix<T, AmbientDim, 1>& start, const Eigen::Matrix<T, AmbientDim, 1>& end) : start{ start }, end{ end } {}
    Ray& operator=(const Ray& other) {
        start = other.start;
        end = other.end;
        return *this;
    }
    operator Eigen::ParametrizedLine<T, AmbientDim>() const {
        return { start, (end - start).normalized() };
    }
};

typedef Ray<float, 3> Ray3f;

template<typename T, int AmbientDim>
Ray<T, AmbientDim> operator*(const Eigen::Matrix<T, AmbientDim + 1, AmbientDim + 1>& mat, const Ray<T, AmbientDim>& ray) {
    return { (mat * ray.start.homogeneous()).hnormalized(), (mat * ray.end.homogeneous()).hnormalized() };
}

template<typename T, int AmbientDim, int Mode>
Ray<T, AmbientDim> operator*(const Eigen::Transform<T, AmbientDim, Mode>& transform, const Ray<T, AmbientDim>& ray) {
    return { transform * ray.start, transform * ray};
}
