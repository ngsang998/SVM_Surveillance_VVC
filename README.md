# SVM_Surveillance_VVC
Copy the file "SVVC_Feature_Extraction.cpp" to the end of function xMotionEstimation() of VVC reference sofware (VTM) to perform feature extraction.  
To perform classify, use the function run_SVM() in svm_classify.c
```python
Input: CurrentPU
Output: lable, MV_top, MV_left, MV_topleft, MV_parent, variance

For each PU -----------------------------------------------------------------------------
If (PU_top != null && PU_left != null && PU_TopLeft != null && PU_Parent != null)
//1. Compute variance value using CalculateVariance()
Variance ← CalculateVariance(CurrentPU)
//2. Compute motion vector length using getMV()
MV_Left  ← getMV(PU_Left)	
MV_Top  ← getMV(PU_Top)
MV_TopLeft  ← getMV(PU_TopLeft)
MV_Parent  ← getMV(PU_Parent)
//3. Optimal search range is equal to motion vector length of current PU
OSR ← getMV(PU_Parent)
if(OSR > 4)
	lable ← 1
           else
lable ← 0
         //4. Write to text file if using for training process
        file.writeLine(“lable 1:MV_Left 2:MV_Top 3:MV_TopLeft 4:MV_Parent 5:Variance” )

```
