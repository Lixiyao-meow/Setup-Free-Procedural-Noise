#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class PoissonDisk{

public:

	template<typename T>
	struct array3d{
	public:
		T* data;
		int w, h, l;
	
		array3d(){
			data = nullptr;
			w = 0;
			h = 0;
			l = 0;
		}

		array3d(const array3d<T>& other){
			int size = other.width * other.height * other.length;
			data = new T[size];
			w = other.width;
			h = other.height;
			l = other.length;
			for(int x=0; x < size; ++x){
				data[x]=other.data[x];
			}
		}

		array3d(int width, int height, int length){
			w = width;
			h = height;
			l = length;
			data = new T [ width * height * length];
		}

		void resize(int width, int height, int length){
			w = width;
			h = height;
			l = length;
			data = new T [ width * height * length];
		}

		~array3d(){
			delete[] data;
		}
		
		inline T& operator()(int x, int y, int z){
			return data[z * w * h + y * w + x];
		}

		inline const T& operator()(int x, int y, int z) const{
			return data[z * w * h + y * w + x];
		}
		
		inline int size() const{
			return w * h * l;
		}
	};


    PoissonDisk(float width, float height, float depth, float radius);
	
	void Samples();
	
	bool IsContains (glm::vec3 v, glm::vec3 area);
	
	glm::vec3 GenerateRandomPointAround (glm::vec3 point, float minDist);
	
	bool IsFarEnough(glm::vec3 sample);
	
	void AddSample (glm::vec3 sample);

	std::vector<glm::vec3> output_samples();

private: 
    int k = 30;  // Maximum number of attempts before marking a sample as inactive.
	glm::vec3 cube;
	float radius;
	float cellSize;
	array3d<glm::vec3> grid;
    std::vector<glm::vec3> activeSamples;

	struct GridPos{
		int x;
		int y;
		int z;
				
		GridPos(glm::vec3 sample, float cellSize){
			x = (int)(sample[0] / cellSize);
			y = (int)(sample[1] / cellSize);
			z = (int)(sample[2] / cellSize);
		};
	};
};
