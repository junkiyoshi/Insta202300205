#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->base_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int span = 20;
	int radius = 90;
	ofColor color;

	while (this->log_list.size() < 18) {
	
		int deg_base = ofRandom(360);
		int deg = deg_base / 24 * 24;

		vector<glm::vec2> log;
		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		log.push_back(location);
		auto next = glm::vec2((radius - 10) * cos(deg * DEG_TO_RAD), (radius - 10) * sin(deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(120 * cos(random_deg * DEG_TO_RAD), 120 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->log_list.push_back(log);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(0.25, 0.6));
		color.setHsb(ofRandom(255), 180, 255);
		this->color_list.push_back(color);
	}

	for (int i = this->log_list.size() - 1; i > -1; i--) {

		auto location = this->log_list[i][this->log_list[i].size() - 1];
		location += this->velocity_list[i];
		this->log_list[i].push_back(location);
		this->velocity_list[i] *= 1.01;

		if (glm::length(location) > 800) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}
		else if (glm::length(location) > 700) {

			this->color_list[i] = ofColor(this->color_list[i], ofMap(glm::length(location), 700, 800, 255, 0));
		}
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		int start_index = this->mesh.getNumVertices() - 1;

		for (int k = 0; k < this->log_list[i].size(); k++) {

			this->mesh.addVertex(glm::vec3(this->log_list[i][k], 0));
			this->mesh.addColor(this->color_list[i]);

			if (k > 0)  {

				this->mesh.addIndex(this->mesh.getNumVertices() - 1); this->mesh.addIndex(this->mesh.getNumVertices() - 2);
			}
		}

		if (i > 0) {

			this->mesh.addIndex(this->mesh.getNumVertices() - 1); this->mesh.addIndex(start_index);
		}
	}

	/*
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->mesh.getNumVertices(); k++) {

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < span) {

				auto alpha = distance < span * 0.25 ? 255 : ofMap(distance, span * 0.25, span, 255, 0);

				if (this->mesh.getColor(i).a < alpha) {

					this->mesh.setColor(i, ofColor(this->mesh.getColor(i), alpha));
				}

				if (this->mesh.getColor(k).a < alpha) {

					this->mesh.setColor(k, ofColor(this->mesh.getColor(k), alpha));
				}

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->mesh.drawWireframe();

	ofFill();
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		//ofDrawCircle(this->mesh.getVertex(i), 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}