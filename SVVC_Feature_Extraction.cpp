const CompArea &area = pu.block(getFirstComponentOfChannel(CHANNEL_TYPE_LUMA));
const Position  posRT = area.topRight();
const Position  posRB = area.bottomRight();
const Position  posLB = area.bottomLeft();
const Position  posLT = area.topLeft();
const PredictionUnit *puLeft = pu.cs->getPURestricted(posLB.offset(-1, 0), pu, CHANNEL_TYPE_LUMA);
const PredictionUnit *puTop = pu.cs->getPURestricted(posRT.offset(0, -1), pu, CHANNEL_TYPE_LUMA);
const PredictionUnit *puTopLeft = pu.cs->getPURestricted(posLT.offset(-1, -1), pu, pu.chType);
int mvCurCU = (int)sqrt(pu.getMotionBuf().height * pu.getMotionBuf().height + pu.getMotionBuf().width * pu.getMotionBuf().width);

int maxSR = 0;
const PredictionUnit *puParent = nullptr;
if (pu.lheight() < 128 && pu.lwidth() < 128 && pu.cs->bestParent->getCU(CHANNEL_TYPE_LUMA) != nullptr)
{
  puParent = pu.cs->bestParent->getCU(CHANNEL_TYPE_LUMA)->firstPU;
}
if (puParent != nullptr)
{
  pu.mvParentAvailable = true;
  pu.mvParent = (int)sqrt(puParent->getMotionBuf().height * puParent->getMotionBuf().height + puParent->getMotionBuf().width * puParent->getMotionBuf().width);
  if (pu.mvParent > maxSR)
    maxSR = pu.mvParent;
  pu.cs->slice->getPic()->sumParent += 1;
  if (abs(pu.mvParent - mvCurCU) <= 8)
  {
    pu.cs->slice->getPic()->sumParentEqualCur += 1;
  }
}

if (puLeft != nullptr)
{
  pu.mvLeftAvailable = true;
  pu.mvLeft = (int)sqrt(puLeft->getMotionBuf().height * puLeft->getMotionBuf().height + puLeft->getMotionBuf().width * puLeft->getMotionBuf().width);
  pu.cs->slice->getPic()->sumMvLeft += 1;
  if (abs(pu.mvLeft - mvCurCU) <= 8)
  {
    pu.cs->slice->getPic()->sumMvLeftEqualCur += 1;
  }
}
if (puTop != nullptr)
{
  pu.mvTopAvailable = true;
  pu.mvTop = (int)sqrt(puTop->getMotionBuf().height * puTop->getMotionBuf().height + puTop->getMotionBuf().width * puTop->getMotionBuf().width);
  pu.cs->slice->getPic()->sumMvTop += 1;
  if (abs(pu.mvTop - mvCurCU) <= 8)
  {
    pu.cs->slice->getPic()->sumMvTopEqualCur += 1;
  }
}
if (puTopLeft != nullptr)
{
  pu.mvTopLeftAvailable = true;
  pu.mvTopLeft = (int)sqrt(puTopLeft->getMotionBuf().height * puTopLeft->getMotionBuf().height + puTopLeft->getMotionBuf().width * puTopLeft->getMotionBuf().width);
  pu.cs->slice->getPic()->sumMvTopLeft += 1;
  if (abs(pu.mvTopLeft - mvCurCU) <= 8)
  {
    pu.cs->slice->getPic()->sumMvTopLeftEqualCur += 1;
  }
}
if (puParent != nullptr && puTop != nullptr && puLeft != nullptr && puTopLeft != nullptr && pu.checkedParentNeighbor == false)
{
  int puHeight = pu.lheight();
  int puWidth = pu.lwidth();
  int sumValue = 0;
  for (int i = pu.ly(); i < pu.ly() + pu.lheight(); i++)
  {
    for (int j = pu.lx(); j < pu.lx() + pu.lwidth(); j++)
    {
      sumValue += pu.cu->slice->getPic()->getOrigBuf().Y().at(j, i);
    }
  }
  double meanValue;
  int puSize = puHeight * puWidth;
  meanValue = sumValue / puSize;
  double sumSquareError = 0.00;
  for (int i = pu.ly(); i < pu.ly() + pu.lheight(); i++)
  {
    for (int j = pu.lx(); j < pu.lx() + pu.lwidth(); j++)
    {
      double diff = std::abs(pu.cu->slice->getPic()->getOrigBuf().Y().at(j, i) - meanValue);
      sumSquareError += diff * diff;
    }
  }
  int curMVlength = (int)sqrt(pu.getMotionBuf().height * pu.getMotionBuf().height + pu.getMotionBuf().width * pu.getMotionBuf().width);
  int lable = 1;
  if (curMVlength > 4)
    lable = -1;
  double variance = sumSquareError / puSize;
  ofstream FeatureFile("training_example.txt", ios::app);

  FeatureFile << lable << " 1:" <<pu.mvLeft << " 2:" << pu.mvTop << " 3:" << pu.mvTopLeft << " 4:" << pu.mvParent << " 5:" << variance << "\n";
  FeatureFile.close();
  pu.checkedParentNeighbor = true;
}