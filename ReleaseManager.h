#pragma once
#include <d3d12.h>

class ReleaseManager {
public:
	ReleaseManager(ID3D12Resource* resource):
		resource_(resource)
	{}

	~ReleaseManager() {
		if (resource_) {
			resource_->Release();
		}
	}

	ID3D12Resource* Get() { return resource_; }

private:
	ID3D12Resource* resource_;
};