clear; clc;
% 用多元线性回归进行复杂形式的函数拟合

% 生成训练数据
rng(1); % 随机数种子
x1 = rand(100, 1);
x2 = rand(100, 1);
x3 = rand(100, 1);
x4 = rand(100, 1);
xData = [x1, x2, x3, x4];
% 生成带高斯噪音的 y 数据
A = 2;  % 噪音幅度
yData = 3.*x1 + 4.*x2 + 5.*x3 + 8.*x4 + 6 + A.*randn(100, 1);
% 理论上：w = [3, 4, 5, 8], b = 6
% yData = xData * (w') + b;

% 模型超参数设置
eta = 0.01;                         % 学习率
tol = 1e-3;                         % 两次迭代代价函数之差 < tol，则迭代终止
maxIterationNum = 10000;            % 最大迭代次数
w0 = rand(1, 4); b0 = rand(1, 1);   % 随机初始化初值

% 新建 1 个模型
model = LinearRegression(eta, tol, maxIterationNum);
% 拟合
model = model.Fit(xData, yData, w0, b0);

fprintf("拟合结果：\n");
w = model.w
b = model.b
fprintf("理论上：\n");
w = [3, 4, 5, 8]
b = 6

% 生成测试数据
rng(2);
X1 = rand(100, 1);
X2 = rand(100, 1);
X3 = rand(100, 1);
X4 = rand(100, 1);
XData = [x1, x2, x3, x4];

YData = 3.*x1 + 4.*x2 + 5.*x3 + 8.*x4 + 6;  % 理论值
Y_predict = model.Predict(XData);           % 预测值

% 数据可视化
scatter(YData, Y_predict, 'filled');
hold on;
X = linspace(min(YData), max(YData), 101);
plot(X, X, '--');   % 直线 y = x
xlabel('实际值');
ylabel('预测值');
grid on;

