# Implementation and Calibration of an Heston Model

---

## Files

- **`Model.h` / `Model.cpp`**  
  Contains the base class and core logic for financial models.

---

## Class Structure
class Model
│
├── class BSModel : public Model
│	// Black-Scholes implementation
│
└── class HestonModel : public Model
	// Heston model implementation