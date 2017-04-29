#ifndef MESH_BUILDER_APP_H
#define MESH_BUILDER_APP_H

#include <jni.h>
#include <memory>
#include <mutex>
#include <pthread.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <tango_client_api.h>  // NOLINT
#include <tango_3d_reconstruction_api.h>
#include <tango_support_api.h>

#include "scene.h"

namespace oc {

    struct GridIndex {
        Tango3DR_GridIndex indices;

        bool operator==(const GridIndex &other) const;
    };

    struct GridIndexHasher {
        std::size_t operator()(const oc::GridIndex &index) const {
            std::size_t val = std::hash<int>()(index.indices[0]);
            val = hash_combine(val, std::hash<int>()(index.indices[1]));
            val = hash_combine(val, std::hash<int>()(index.indices[2]));
            return val;
        }

        static std::size_t hash_combine(std::size_t val1, std::size_t val2) {
            return (val1 << 1) ^ val2;
        }
    };

    class MeshBuilderApp {
    public:
        MeshBuilderApp();
        ~MeshBuilderApp();
        void OnCreate(JNIEnv *env, jobject caller_activity);
        void OnPause();
        void OnTangoServiceConnected(JNIEnv *env, jobject binder, double res, double dmin, double dmax,
                                     int noise, bool land, std::string dataset);
        void onPointCloudAvailable(TangoPointCloud *point_cloud);
        void onFrameAvailable(TangoCameraId id, const TangoImageBuffer *buffer);
        void OnSurfaceCreated();
        void OnSurfaceChanged(int width, int height);
        void OnDrawFrame();
        void OnToggleButtonClicked(bool t3dr_is_running);
        void OnClearButtonClicked();
        void Load(std::string filename);
        void Save(std::string filename, std::string dataset);
        float CenterOfStaticModel(bool horizontal);
        void SetView(float p, float y, float mx, float my, bool g) { pitch = p; yaw = y; gyro = g;
                                                                            movex = mx; movey = my;}
        void SetZoom(float value) { zoom = value; }

    private:
        void TangoSetupConfig();
        Tango3DR_ReconstructionContext TangoSetup3DR(double res, double dmin, double dmax, int noise);
        void TangoConnectCallbacks();
        void TangoConnect();
        void TangoDisconnect();
        void DeleteResources();
        void MeshUpdate(Tango3DR_ImageBuffer t3dr_image, Tango3DR_GridIndexArray *t3dr_updated);
        std::string GetFileName(int index, std::string extension);

        bool t3dr_is_running_;
        Tango3DR_ReconstructionContext t3dr_context_;
        Tango3DR_CameraCalibration t3dr_intrinsics_;
        glm::mat4 image_matrix;
        glm::quat image_rotation;
        std::mutex binder_mutex_;
        std::mutex render_mutex_;

        bool point_cloud_available_;
        TangoSupportPointCloudManager* point_cloud_manager_;
        TangoPointCloud* front_cloud_;
        glm::mat4 point_cloud_matrix_;

        Scene main_scene_;
        TangoConfig tango_config_;
        std::unordered_map<GridIndex, SingleDynamicMesh*, GridIndexHasher> meshes_;

        std::string dataset_;
        int poses_;

        bool gyro;
        bool landscape;
        float movex;
        float movey;
        float pitch;
        float yaw;
        float zoom;
    };
}

#endif
